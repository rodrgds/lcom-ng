"""Optional lcom.json, resolved once for web, native exports and bundles."""
import copy
import json
from pathlib import Path, PurePosixPath
import re

DEFAULT_MOBILE = {
    'orientation': 'auto', 'fullscreen': False,
    'controls': {'keyboard': True, 'mouseButtons': True, 'quickKeys': ['ESC', 'SPACE', 'ENTER'],
                 'joystick': True, 'look': False, 'dpad': False, 'layout': 'panel', 'pointer': 'drag',
                 'touch': 'auto', 'touchClick': True, 'pads': [], 'actions': [], 'repeatKeys': [], 'repeatReleaseKeys': [],
                 'toolbar': 'auto', 'keyboardPosition': 'auto', 'tapDuration': 0,
                 'cursor': {'x': -1, 'y': -1, 'maxX': -1, 'maxY': -1,
                            'minX': 0, 'minY': 0, 'bounds': 'clamp'}},
    'theme': {'accent': '#72d6b0', 'keyboardAccent': '#72d6b0', 'button': '#222222',
              'text': '#eeeeee', 'font': 'sans'},
}
KEYS = {'ESC','SPACE','ENTER','KP_ENTER','BACKSPACE','TAB','UP','DOWN','LEFT','RIGHT','LSHIFT','LCTRL','LALT',
        'HOME','END','PAGEUP','PAGEDOWN','DELETE','INSERT',
        'MINUS','EQUAL','LBRACKET','RBRACKET','BACKSLASH','SEMICOLON','APOSTROPHE','BACKQUOTE','COMMA','PERIOD','SLASH', *'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789',
        *(f'F{i}' for i in range(1,13)), *(f'KP{i}' for i in range(10))}
CORNERS = ['top-left','top-right','bottom-left','bottom-right']


def object_value(value, path, allowed):
    if not isinstance(value, dict): raise ValueError(f'{path} must be an object')
    unknown = value.keys() - set(allowed)
    if unknown: raise ValueError(f'{path}: unknown setting {sorted(unknown)[0]}')
    return value


def choice(value, path, values):
    if value not in values: raise ValueError(f'{path} must be one of: {", ".join(values)}')
    return value


def boolean(value, path):
    if type(value) is not bool: raise ValueError(f'{path} must be true or false')
    return value


def string(value, path):
    if not isinstance(value, str) or not value.strip() or '\0' in value:
        raise ValueError(f'{path} must be a non-empty string')
    return value


def instructions(value):
    """Author-written help, independent of input bindings and guest behavior."""
    def text(value, path, limit):
        if len(string(value, path)) > limit: raise ValueError(f'{path} must contain at most {limit} characters')
    def items(value, path, limit):
        if not isinstance(value, list) or not 1 <= len(value) <= limit:
            raise ValueError(f'{path} must contain between 1 and {limit} items')
        return enumerate(value)

    object_value(value, 'instructions', ['sections'])
    for index, section in items(value.get('sections'), 'instructions.sections', 8):
        path = f'instructions.sections[{index}]'
        object_value(section, path, ['title', 'text', 'controls'])
        if not ('text' in section or 'controls' in section): raise ValueError(path + ' requires text or controls')
        for field, limit in [('title', 80), ('text', 1000)]:
            if field in section: text(section[field], path + '.' + field, limit)
        if 'controls' not in section: continue
        for index, control in items(section['controls'], path + '.controls', 16):
            row = f'{path}.controls[{index}]'
            object_value(control, row, ['action', 'bindings', 'note'])
            text(control.get('action'), row + '.action', 80)
            if 'note' in control: text(control['note'], row + '.note', 240)
            for index, binding in items(control.get('bindings'), row + '.bindings', 8):
                source = f'{row}.bindings[{index}]'
                object_value(binding, source, ['keys', 'label', 'platforms'])
                if sum(field in binding for field in ('keys', 'label')) != 1:
                    raise ValueError(source + ' requires exactly one of keys or label')
                if 'label' in binding: text(binding['label'], source + '.label', 120)
                if 'keys' in binding:
                    for _, key in items(binding['keys'], source + '.keys', 6): text(key, source + '.keys', 32)
                if 'platforms' in binding:
                    for _, platform in items(binding['platforms'], source + '.platforms', 5):
                        choice(platform, source + '.platforms', ['macos', 'windows', 'linux', 'ios', 'android'])
                    if len(set(binding['platforms'])) != len(binding['platforms']):
                        raise ValueError(source + '.platforms cannot contain duplicates')
    return copy.deepcopy(value)


def icon_path(path):
    path = Path(path).resolve(strict=True)
    data = path.read_bytes()
    if len(data) < 24 or data[:8] != b'\x89PNG\r\n\x1a\n' or data[12:16] != b'IHDR':
        raise ValueError('icon must be a square PNG image')
    w,h = int.from_bytes(data[16:20],'big'),int.from_bytes(data[20:24],'big')
    if w != h or w < 48: raise ValueError('icon must be square and at least 48 pixels wide')
    return path


def preview_path(path):
    try:
        path = Path(path).resolve(strict=True)
        data = path.read_bytes()
    except OSError as error:
        raise ValueError(f'preview image could not be read: {path}') from error
    if (len(data) < 24 or data[:8] != b'\x89PNG\r\n\x1a\n' or data[12:16] != b'IHDR'
            or min(int.from_bytes(data[16:20], 'big'), int.from_bytes(data[20:24], 'big')) < 48):
        raise ValueError('preview must be a PNG image at least 48 pixels in each dimension')
    return path


def load(project, explicit=None):
    """Missing implicit config is valid. Explicit missing paths and typos are errors."""
    project = Path(project).resolve()
    path = Path(explicit).resolve(strict=True) if explicit else project / 'lcom.json'
    data = json.loads(path.read_text()) if path.is_file() else {}
    object_value(data, str(path), ['$schema','version','name','icon','description','instructions','preview','archetype','mobile','build'])
    if 'version' in data and (type(data['version']) is not int or data['version'] != 1):
        raise ValueError('lcom.json version must be 1')
    result = {'name': string(data.get('name',project.name),'name'), 'icon': None, 'preview': None, 'description': '', 'archetype': None,
              'mobile': copy.deepcopy(DEFAULT_MOBILE), 'build': {}}
    if 'instructions' in data: result['instructions'] = instructions(data['instructions'])
    if 'icon' in data: result['icon'] = icon_path(path.parent / string(data['icon'],'icon'))
    if 'description' in data:
        result['description'] = string(data['description'], 'description').strip()
        if len(result['description']) > 500: raise ValueError('description must contain at most 500 characters')
    if 'archetype' in data:
        result['archetype'] = string(data['archetype'], 'archetype').strip()
        if len(result['archetype']) > 64 or any(ord(char) < 32 for char in result['archetype']):
            raise ValueError('archetype must contain at most 64 characters on one line')
    if 'preview' in data: result['preview'] = preview_path(path.parent / string(data['preview'], 'preview'))
    mobile = object_value(data.get('mobile',{}),'mobile',['orientation','fullscreen','controls','theme'])
    for key in ('orientation','fullscreen'):
        if key in mobile:
            result['mobile'][key] = choice(mobile[key],'mobile.orientation',['auto','landscape','portrait']) if key=='orientation' else boolean(mobile[key],'mobile.fullscreen')
    controls = object_value(mobile.get('controls',{}),'mobile.controls',DEFAULT_MOBILE['controls'])
    for key,value in controls.items():
        target = 'mobile.controls.'+key
        if key in ('keyboard','mouseButtons','touchClick'): boolean(value,target)
        elif key in ('joystick','look','dpad'):
            if type(value) is bool:
                if value and key != 'joystick': value = {'side':'right' if key=='look' else 'left'}
            else:
                object_value(value,target,['side','mode','axes'] if key=='joystick' else ['side','keys','axes'] if key=='dpad' else ['side','sensitivity'])
                value = dict(value)
            if isinstance(value,dict):
                value['side'] = choice(value.get('side','right' if key=='look' else 'left'),target+'.side',['left','right'])
                if key=='joystick': value['mode'] = choice(value.get('mode','arrows'),target+'.mode',['arrows','wasd','mouse'])
                if key=='dpad': value['keys'] = choice(value.get('keys','arrows'),target+'.keys',['arrows','wasd'])
                if key in ('joystick','dpad'):
                    value['axes'] = choice(value.get('axes','both'),target+'.axes',['both','horizontal','vertical'])
                if key=='look':
                    sensitivity=value.get('sensitivity',1)
                    if type(sensitivity) not in (int,float) or not 0.1 <= sensitivity <= 5: raise ValueError(target+'.sensitivity must be between 0.1 and 5')
                    value['sensitivity']=sensitivity
        elif key in ('quickKeys','repeatKeys','repeatReleaseKeys'):
            if not isinstance(value,list) or len(value)>16 or any(not isinstance(v,str) or v not in KEYS for v in value):
                raise ValueError(target+' must contain up to 16 guest key names, for example ESC or SPACE')
            if len(set(value)) != len(value): raise ValueError(target+' cannot contain duplicate keys')
        elif key=='layout': choice(value,target,['panel','overlay'])
        elif key=='pointer': choice(value,target,['drag','move','direct','off'])
        elif key=='touch': choice(value,target,['auto','press','tap','tap-hold','hold-drag','move'])
        elif key=='toolbar': choice(value,target,['auto',*CORNERS])
        elif key=='keyboardPosition': choice(value,target,['auto','bottom','left','right'])
        elif key=='tapDuration':
            if type(value) is not int or not 0<=value<=250: raise ValueError(target+' must be an integer between 0 and 250 milliseconds')
        elif key=='pads':
            if not isinstance(value,list) or len(value)>2: raise ValueError(target+' must contain up to two pads')
            pads=[]
            for p in value:
                object_value(p,target,['side','mode','style','keys','sensitivity','axes','maxAngle','deadZone'])
                p={**{'side':'left','mode':'arrows','style':'dpad','sensitivity':1,
                      'axes':'horizontal' if p.get('style')=='wheel' else 'both'},**p}
                choice(p['side'],target+'.side',['left','right'])
                choice(p['mode'],target+'.mode',['arrows','wasd','keypad','mouse'])
                choice(p['style'],target+'.style',['dpad','stick','wheel'])
                choice(p['axes'],target+'.axes',['both','horizontal','vertical'])
                if type(p['sensitivity']) not in (int,float) or not 0.1<=p['sensitivity']<=10:
                    raise ValueError(target+'.sensitivity must be between 0.1 and 10')
                if p['mode']=='mouse' and (p['style']!='stick' or 'keys' in p):
                    raise ValueError(target+': mouse pads require stick style and no keys')
                if p['style']=='wheel':
                    if p['axes']!='horizontal': raise ValueError(target+': wheels use horizontal steering only')
                    for field,default,minimum,maximum in [('maxAngle',135,45,270),('deadZone',8,0,30)]:
                        value=p.get(field,default)
                        if type(value) not in (int,float) or not minimum<=value<=maximum:
                            raise ValueError(target+'.'+field+f' must be between {minimum} and {maximum} degrees')
                        p[field]=value
                elif 'maxAngle' in p or 'deadZone' in p:
                    raise ValueError(target+': maxAngle and deadZone require wheel style')
                if 'keys' in p:
                    object_value(p['keys'],target+'.keys',['up','left','down','right'])
                    required={'left','right'} if p['style']=='wheel' else {'up','left','down','right'}
                    if not required.issubset(p['keys']) or any(not isinstance(k,str) or k not in KEYS for k in p['keys'].values()):
                        raise ValueError(target+'.keys must map '+('left and right' if p['style']=='wheel' else 'all four directions')+' to guest key names')
                pads.append(p)
            value=pads
        elif key=='actions':
            if not isinstance(value,list) or len(value)>8: raise ValueError(target+' must contain up to eight actions')
            positions=[]
            for action in value:
                object_value(action,target,['side','position','label','key','mouseButton','wheel','icon'])
                if 'icon' in action: choice(action['icon'],target+'.icon',['accelerator','brake','reverse'])
                if 'side' in action: choice(action['side'],target+'.side',['left','right'])
                if 'position' in action: choice(action['position'],target+'.position',CORNERS)
                elif 'side' not in action: raise ValueError(target+' requires side or position')
                label=string(action.get('label'),target+'.label')
                if len(label)>16: raise ValueError(target+'.label must contain up to 16 characters')
                if sum(field in action for field in ('key','mouseButton','wheel')) != 1: raise ValueError(target+' requires exactly one of key, mouseButton or wheel')
                if 'key' in action: choice(action['key'],target+'.key',sorted(KEYS))
                elif 'mouseButton' in action: choice(action['mouseButton'],target+'.mouseButton',['left','right','middle'])
                elif type(action['wheel']) is not int or not -127 <= action['wheel'] <= 127 or action['wheel'] == 0:
                    raise ValueError(target+'.wheel must be a nonzero integer from -127 to 127')
                positions.append(action.get('position','bottom-'+action.get('side','right')))
            if any(positions.count(position)>4 for position in positions): raise ValueError(target+' allows up to four actions per position')
        elif key=='cursor':
            object_value(value,target,[*DEFAULT_MOBILE['controls']['cursor'],'desktopScale','desktopFractional'])
            value={**DEFAULT_MOBILE['controls']['cursor'],**value}
            for field,coordinate in value.items():
                if field=='desktopFractional':
                    boolean(coordinate,target+'.desktopFractional'); continue
                if field=='desktopScale':
                    if type(coordinate) not in (int,float) or coordinate not in (0.5,1,2):
                        raise ValueError(target+'.desktopScale must be 0.5, 1 or 2')
                    continue
                if field=='bounds':
                    choice(coordinate,target+'.bounds',['clamp','reject']); continue
                if type(coordinate) is not int or not -1 <= coordinate <= 16384:
                    raise ValueError(target+'.'+field+' must be an integer between -1 and 16384')
            for axis,maximum,minimum in [('x','maxX','minX'),('y','maxY','minY')]:
                if value[minimum]<0 or (value[maximum]>=0 and value[minimum]>value[maximum]):
                    raise ValueError(target+'.'+minimum+' must be between zero and '+maximum)
                if value[axis]>=0 and value[axis]<value[minimum]: raise ValueError(target+'.'+axis+' must not be below '+minimum)
                if value[axis]>=0 and value[maximum]>=0 and value[axis]>value[maximum]:
                    raise ValueError(target+'.'+axis+' must not exceed '+maximum)
        result['mobile']['controls'][key] = value
    theme=object_value(mobile.get('theme',{}),'mobile.theme',DEFAULT_MOBILE['theme'])
    for key,value in theme.items():
        if key=='font': choice(value,'mobile.theme.font',['sans','mono'])
        elif not isinstance(value,str) or not re.fullmatch('#[0-9a-fA-F]{6}',value): raise ValueError('mobile.theme.'+key+' must be a #RRGGBB color')
        result['mobile']['theme'][key] = value
    if 'accent' in theme and 'keyboardAccent' not in theme: result['mobile']['theme']['keyboardAccent']=theme['accent']
    build=object_value(data.get('build',{}),'build',['sources','include','assets','assetRoot','arguments','libraries','environment','pathAliases','emulateFunctionPointerCasts','cStandard'])
    for key,value in build.items():
        if key=='cStandard':
            if value not in ('gnu89','gnu99','gnu11','gnu17'): raise ValueError('build.cStandard must be gnu89, gnu99, gnu11 or gnu17')
        elif key=='emulateFunctionPointerCasts':
            value=boolean(value,'build.'+key)
        elif key=='environment':
            if not isinstance(value,dict): raise ValueError('build.environment must be an object of strings')
            for name,setting in value.items():
                if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_]*',name) or not isinstance(setting,str) or '\0' in setting:
                    raise ValueError('build.environment must contain valid variable names and string values without NUL')
        elif key=='pathAliases':
            if not isinstance(value,dict) or len(value)>64:
                raise ValueError('build.pathAliases must be an object with up to 64 directory prefixes and relative destinations')
            aliases={}
            for origin,destination in value.items():
                if not isinstance(origin,str) or not isinstance(destination,str) or not destination or any(c == '\\' or ord(c) < 32 for c in origin+destination):
                    raise ValueError('build.pathAliases requires POSIX paths without control characters')
                before,after=PurePosixPath(origin),PurePosixPath(destination)
                if str(before)=='.' or not str(before).strip('/') or after.is_absolute() or '..' in before.parts or '..' in after.parts:
                    raise ValueError('build.pathAliases requires nonempty directory prefixes and relative destinations inside the asset root')
                normalized=str(before).rstrip('/')
                if normalized in aliases: raise ValueError('build.pathAliases contains duplicate normalized roots')
                aliases[normalized]=str(after)
            value=aliases
        elif key=='assetRoot': string(value,'build.'+key)
        elif not isinstance(value,list) or any(not isinstance(v,str) or not v or '\0' in v for v in value): raise ValueError('build.'+key+' must be an array of strings')
        result['build'][key]=value
    resolved_controls=result['mobile']['controls']
    if set(resolved_controls['repeatKeys']) & set(resolved_controls['repeatReleaseKeys']):
        raise ValueError('mobile.controls.repeatReleaseKeys must not overlap repeatKeys')
    other_sides=[resolved_controls[k]['side'] for k in ('look','dpad') if isinstance(resolved_controls[k],dict)]
    other_sides += [p['side'] for p in resolved_controls['pads']]
    if resolved_controls['joystick'] is True and other_sides:
        if 'joystick' not in controls:
            resolved_controls['joystick']=False
        else:
            free=[side for side in ('left','right') if side not in other_sides]
            if not free: raise ValueError('mobile.controls.joystick: no free side for another pad')
            resolved_controls['joystick']={'mode':'mouse','side':free[0]}
    occupied=[]
    for key in ('joystick','look','dpad'):
        pad=result['mobile']['controls'][key]
        if isinstance(pad,dict):
            if pad['side'] in occupied: raise ValueError('mobile.controls: only one pad can occupy each side')
            occupied.append(pad['side'])
    for pad in resolved_controls['pads']:
        if pad['side'] in occupied: raise ValueError('mobile.controls: only one pad can occupy each side')
        occupied.append(pad['side'])
    # Source paths are relative to the project, identity artwork to the config.
    return result


def resolve(args):
    config = load(args.project, getattr(args,'config',None))
    args.name = string(args.name,'--name') if args.name is not None else config['name']
    args.icon = icon_path(args.icon) if getattr(args,'icon',None) else config['icon']
    if getattr(args,'orientation',None) is not None: config['mobile']['orientation']=args.orientation
    args.orientation=config['mobile']['orientation']
    args.assets_configured = bool(args.asset) or 'assets' in config['build']
    for attr,key in [('source','sources'),('include','include'),('asset','assets'),('asset_root','assetRoot'),('arg','arguments'),('library','libraries')]:
        if not getattr(args,attr) and key in config['build']:
            setattr(args,attr,Path(config['build'][key]) if attr=='asset_root' else config['build'][key])
    args.c_standard=config['build'].get('cStandard','gnu11')
    args.path_aliases=config['build'].get('pathAliases',{})
    config.update(name=args.name, icon=args.icon)
    return config


def runtime_environment(config):
    environment=dict(config['build'].get('environment',{}))
    aliases=config['build'].get('pathAliases',{})
    if aliases:
        environment['LCOM_COMPAT_PATH_ALIAS_COUNT']=str(len(aliases))
        for index,(origin,destination) in enumerate(aliases.items()):
            environment[f'LCOM_COMPAT_PATH_ALIAS_{index}_FROM']=origin
            environment[f'LCOM_COMPAT_PATH_ALIAS_{index}_TO']=destination
    return environment


def public(config):
    return {'name':config['name'], 'description':config['description'], 'archetype':config['archetype'],
            **({'instructions':config['instructions']} if 'instructions' in config else {}),
            'preview':'preview.png' if config['preview'] else None, 'icon':'icon.png' if config['icon'] else None,'mobile':config['mobile'],
            'environment':runtime_environment(config)}


def cpp_string(value):
    return '"' + ''.join('\\%03o' % byte for byte in value.encode('utf-8')) + '"'


def write_native(config, path):
    """Compile the resolved settings into the standalone player, without a JSON runtime."""
    m=config['mobile']; c=m['controls']; t=m['theme']
    lines=['\n#include "embedded/Controls.hpp"', '#include <array>', '#include <utility>', 'namespace game {',
           'inline const lcom::embedded::ControlConfig controls = [] {',
           '  lcom::embedded::ControlConfig c;']
    values={'name':config['name'],'orientation':m['orientation'],'layout':c['layout'],'pointer':c['pointer'],'touch':c['touch'],'font':t['font'],
            'toolbar':c['toolbar'],'keyboardPosition':c['keyboardPosition'],'tapDuration':c['tapDuration'],
            'touchClick':c['touchClick'],'fullscreen':m['fullscreen'],'keyboard':c['keyboard'],'mouseButtons':c['mouseButtons'],
            'joystickMenu':c['joystick'] is True}
    for key,value in values.items(): lines.append(f'  c.{key} = ' + (cpp_string(value) if isinstance(value,str) else json.dumps(value)) + ';')
    lines.append('  c.quickKeys = {' + ','.join(json.dumps(v) for v in c['quickKeys']) + '};')
    lines.append('  c.repeatReleaseKeys = {' + ','.join(json.dumps(v) for v in c['repeatReleaseKeys']) + '};')
    lines.append('  c.repeatKeys = {' + ','.join(json.dumps(v) for v in c['repeatKeys']) + '};')
    lines.append('  c.cursor = {' + ','.join(str(c['cursor'][key]) for key in ('x','y','maxX','maxY','minX','minY')) + ','+str(c['cursor']['bounds']=='reject').lower()+'};')
    for key in ('joystick','look','dpad'):
        p=c[key]
        if isinstance(p,dict):
            mode=p.get('mode',p.get('keys','mouse'))
            lines.append(f'  c.{key} = {{true, {json.dumps(mode)}, {json.dumps(p["side"])}, {float(p.get("sensitivity",1))}f, {str(key == "dpad").lower()}}};')
            if key in ('joystick','dpad'): lines.append(f'  c.{key}.axes = {cpp_string(p.get("axes","both"))};')
    for p in c['pads']:
        keys='{'+','.join(cpp_string(p.get('keys',{}).get(k,'')) for k in ('left','up','right','down'))+'}'
        lines.append('  c.pads.push_back({true,'+cpp_string(p['mode'])+','+cpp_string(p['side'])+','+str(float(p['sensitivity']))+'f,'+str(p['style']=='dpad').lower()+','+keys+'});')
        lines.append('  c.pads.back().axes = '+cpp_string(p['axes'])+';')
        if p['style']=='wheel':
            lines.append('  c.pads.back().wheel = true;')
            for field in ('maxAngle','deadZone'): lines.append(f'  c.pads.back().{field} = {float(p[field])}f;')
    for a in c['actions']:
        side=a.get('side',a.get('position','bottom-right').split('-')[1])
        lines.append('  c.actions.push_back({'+','.join(cpp_string(side if k=='side' else a.get(k,'')) for k in ('side','label','key'))+','+str({'left':1,'right':2,'middle':4}.get(a.get('mouseButton'),0))+'});')
        lines.append('  c.actions.back().position = '+cpp_string(a.get('position',''))+';')
        if a.get('icon'): lines.append('  c.actions.back().icon = '+cpp_string(a['icon'])+';')
        if a.get('wheel'): lines.append('  c.actions.back().wheel = '+str(a['wheel'])+';')
    for key in ('accent','keyboardAccent','button','text'): lines.append(f'  c.{key} = 0x{t[key][1:]};')
    lines += ['  return c;', '}();']
    environment=runtime_environment(config)
    lines.append(f'inline const std::array<std::pair<const char *, const char *>, {len(environment)}> environment{{{{')
    for name,value in environment.items(): lines.append('  {'+cpp_string(name)+','+cpp_string(value)+'},')
    lines += ['}};', '}']
    with Path(path).open('a') as f: f.write('\n'.join(lines)+'\n')


if __name__ == '__main__':
    import argparse
    parser=argparse.ArgumentParser()
    parser.add_argument('project',type=Path); parser.add_argument('--config',type=Path)
    parser.add_argument('--identity-output',type=Path); parser.add_argument('--name'); parser.add_argument('--icon',type=Path)
    args=parser.parse_args()
    try:
        config=load(args.project,args.config)
        if args.name is not None: config['name']=string(args.name,'name')
        if args.icon: config['icon']=icon_path(args.icon)
        if args.identity_output:
            args.identity_output.write_bytes((config['name']+'\0'+(str(config['icon']) if config['icon'] else '')+'\0').encode())
        else: print(json.dumps({**public(config),'icon':str(config['icon']) if config['icon'] else None}))
    except (OSError,ValueError) as e: parser.exit(1,f'lcom.json: {e}\n')
