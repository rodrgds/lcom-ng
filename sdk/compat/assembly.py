"""Lower the supported Minix basic-assembly boundary without editing source files."""
import ast
import hashlib
import json
from pathlib import Path
import re


class AssemblyError(ValueError):
    pass


# Keep literals intact and blank comments without moving source coordinates.
_LEXEMES = re.compile(r'"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'|/\*.*?\*/|//[^\n]*', re.S)
_ASM = re.compile(r'\b(?:asm|__asm|__asm__)\b\s*(?:(?:volatile|__volatile|__volatile__|goto)\s*)*\(')
_STRINGS = re.compile(r'\s*((?:"(?:\\.|[^"\\])*"\s*)+)', re.S)


def _split_operands(text, separator):
    """Split GNU operand groups without interpreting their C expressions."""
    parts, start, depth, quote, escaped = [], 0, 0, None, False
    for index, char in enumerate(text):
        if quote:
            if escaped: escaped = False
            elif char == "\\": escaped = True
            elif char == quote: quote = None
        elif char in "\"'": quote = char
        elif char in "([{": depth += 1
        elif char in ")]}": depth -= 1
        elif char == separator and depth == 0:
            parts.append(text[start:index].strip())
            start = index + 1
        if depth < 0:
            raise AssemblyError('unbalanced assembly operand')
    if depth or quote:
        raise AssemblyError('unbalanced assembly operand')
    parts.append(text[start:].strip())
    return parts


def _operand(text, constraint):
    match = re.fullmatch(r'"([^"\\]*)"\s*\((.*)\)', text.strip(), re.S)
    if not match or match[1] != constraint or not match[2].strip():
        raise AssemblyError('unsupported byte-port operand constraint')
    # Parenthesis/quote balancing is checked independently of the expression.
    _split_operands('(' + match[2] + ')', ',')
    return match[2].strip()


def _byte_port(template, operands, qualifiers):
    """Recognize proven AL/DX or AL/imm8 instructions, never infer intent."""
    instruction = ' '.join(template.strip().rstrip(';').split()).lower()
    if instruction not in ('inb %1, %0', 'inb %1,%0', 'outb %0, %1', 'outb %0,%1'):
        raise AssemblyError('unsupported byte-port instruction template')
    if not re.search(r'\b(?:volatile|__volatile|__volatile__)\b', qualifiers):
        raise AssemblyError('byte-port assembly requires an explicit volatile qualifier')
    fields = _split_operands(operands, ':')
    if len(fields) not in (3, 4) or fields[0] or (len(fields) == 4 and fields[3]):
        raise AssemblyError('unsupported byte-port outputs, inputs or clobbers')
    outputs = _split_operands(fields[1], ',') if fields[1] else []
    inputs = _split_operands(fields[2], ',') if fields[2] else []
    if instruction.startswith('inb'):
        if len(outputs) != 1 or len(inputs) != 1:
            raise AssemblyError('INB requires one AL output and one port input')
        value, port = _operand(outputs[0], '=a'), _operand(inputs[0], 'Nd')
        return ('do { LCOM_COMPAT_ASM_BYTE(' + value + '); LCOM_COMPAT_ASM_PORT(' + port + '); '
                '__typeof__(&(' + value + ')) __lcom_asm_result = &(' + value + '); '
                '*__lcom_asm_result = (__typeof__(' + value + '))lcom_compat_asm_in8((uint16_t)('
                + port + ')); } while (0)')
    if outputs or len(inputs) != 2:
        raise AssemblyError('OUTB requires no output and AL/port inputs')
    value, port = _operand(inputs[0], 'a'), _operand(inputs[1], 'Nd')
    return ('do { LCOM_COMPAT_ASM_BYTE(' + value + '); LCOM_COMPAT_ASM_PORT(' + port + '); '
            'lcom_compat_asm_out8((uint16_t)(' + port + '), (uint8_t)(' + value + ')); } while (0)')


def lower(source, output):
    """Return original input, or a generated C input and quote-include directory.

    This is deliberately a bounded compiler bridge. Only basic CLI/STI and explicitly volatile byte-port
    AL/imm8-or-DX templates are supported. Other operands/instructions remain diagnostic failures.
    No compiler keyword macros enter authored or system header namespaces.
    """
    source, output = Path(source), Path(output)
    data = source.read_bytes()
    text = data.decode('latin-1')
    clean = _LEXEMES.sub(lambda m: re.sub(r'[^\n]', ' ', m[0])
                        if m[0].startswith(('/*', '//')) else m[0], text)
    # Hide literals from keyword recognition while retaining their offsets.
    code = _LEXEMES.sub(lambda m: re.sub(r'[^\n]', ' ', m[0]), text)
    replacements = []
    for match in _ASM.finditer(clean):
        if not code[match.start():match.start()+3].strip():
            continue
        i, depth, quote, escaped = match.end(), 1, None, False
        while i < len(clean) and depth:
            char = clean[i]
            if quote:
                if escaped: escaped = False
                elif char == '\\': escaped = True
                elif char == quote: quote = None
            elif char in '\"\'': quote = char
            elif char == '(': depth += 1
            elif char == ')': depth -= 1
            i += 1
        body = clean[match.end():i-1]
        strings = _STRINGS.match(body)
        template = ''
        if strings:
            try:
                template = ''.join(ast.literal_eval(value) for value in
                                   re.findall(r'"(?:\\.|[^"\\])*"', strings[1]))
            except (ValueError, SyntaxError):
                pass
        instruction = template.strip().rstrip(';').strip().lower()
        operands = body[strings.end():].strip() if strings else body
        line = text.count('\n', 0, match.start()) + 1
        try:
            if depth or 'goto' in match[0]:
                raise AssemblyError('unbalanced or goto assembly is not supported')
            if instruction in ('cli', 'sti') and not operands:
                replacement = f'lcom_compat_cpu_interrupts({1 if instruction == "sti" else 0})'
            else:
                replacement = _byte_port(template, operands, match[0])
        except AssemblyError as error:
            raise AssemblyError(f'{source}:{line}: unsupported inline assembly; {error}') from error
        replacement += '\n' * text[match.start():i].count('\n')
        replacements.append((match.start(), i, replacement))
    if not replacements:
        return source, []
    for start, end, replacement in reversed(replacements):
        text = text[:start] + replacement + text[end:]
    generated = output / 'assembly-input' / hashlib.sha256(str(source).encode()).hexdigest()[:16] / source.name
    generated.parent.mkdir(parents=True, exist_ok=True)
    prefix = '#include <lcom/compat_asm.h>\n#line 1 ' + json.dumps(str(source)) + '\n'
    generated.write_bytes(prefix.encode('ascii') + text.encode('latin-1'))
    return generated, ['-iquote', str(source.parent)]
