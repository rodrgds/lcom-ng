"""Stable share slugs: the authors' initials, then the game name.

A slug is the public, citable identity of a game (`arcade.rgo.pt/share/afg-ninjix`).
It starts with the first letter of each participant's first name, in alphabetical
order, followed by a normalized game name. Games without evidenced participants
keep the name alone, so an uncertain author never publishes a wrong initial.
"""
import re
import unicodedata

SHARE_SLUG = re.compile(r'[a-z][a-z0-9]*(?:-[a-z0-9]+)*')
MAX_SLUG_LENGTH = 64


def normalize(value):
    """Lowercase ASCII slug fragment for a name or a person's first name."""
    text = unicodedata.normalize('NFKD', str(value))
    text = ''.join(character for character in text if not unicodedata.combining(character))
    return re.sub(r'[^a-z0-9]+', '-', text.lower()).strip('-')


def initials(authors):
    """One letter per participant, sorted, with repeats collapsed."""
    letters = set()
    for author in authors:
        words = normalize(author).split()
        if words:
            letters.add(words[0][0])
    return ''.join(sorted(letters))


def share_slug(authors, name):
    """`afg` + `ninjix` → `afg-ninjix`; no evidenced authors → `ninjix`."""
    prefix = initials(authors or [])
    room = MAX_SLUG_LENGTH - len(prefix) - (1 if prefix else 0)
    tail = normalize(name)[:room].strip('-') or 'game'
    if tail[0].isdigit(): tail = 'game-' + tail
    return f'{prefix}-{tail}' if prefix else tail


def assign_slugs(games):
    """Deterministic collection-wide slugs; collisions gain numeric suffixes.

    `games` holds mappings with `id`, `name` and optional `authors`. Sorting by
    id keeps the assignment stable when a collection is reordered.
    """
    assigned, taken = {}, set()
    for game in sorted(games, key=lambda item: str(item['id'])):
        base = share_slug(game.get('authors') or [], game['name'])
        slug, suffix = base, 2
        while slug in taken:
            slug = f'{base}-{suffix}'
            suffix += 1
        taken.add(slug)
        assigned[game['id']] = slug
    return assigned
