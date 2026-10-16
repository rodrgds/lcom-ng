#!/usr/bin/env python3
"""Capture real Arcade gameplay canvases using authored guest-input sequences.

Run inside Devenv. The checked-in capture plan is independent from temporary
playtest directories. No game state, artwork, RNG or framebuffer is fabricated.
"""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess
import tempfile
import struct
from PIL import Image, ImageDraw

ROOT = next(p for p in (Path.cwd(), *Path(__file__).resolve().parents)
            if (p/'package.json').exists() and (p/'collections/arcade').is_dir())

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def load(path):
    return json.loads(path.read_text())

def origin_metadata(game_id, capture):
    return {'origin': capture['source'], 'game': game_id,
            'sourceCanvasSha256': capture['originalSha256'],
            'replay': capture['replay'], 'replaySha256': capture['replaySha256'],
            'gameFiles': capture['gameFiles'], 'framebuffer': capture['framebuffer'],
            'crop': capture.get('crop')}

def embed_origin(path, metadata):
    """Add EXIF without recompressing the real captured image pixels."""
    data = path.read_bytes()
    if data[:4] != b'RIFF' or data[8:12] != b'WEBP':
        raise ValueError('Expected a WebP capture')
    chunks = []
    offset = 12
    while offset < len(data):
        tag, size = data[offset:offset+4], struct.unpack('<I', data[offset+4:offset+8])[0]
        value = data[offset+8:offset+8+size]
        if tag != b'EXIF':
            chunks.append((tag, value))
        offset += 8 + size + (size & 1)
    if chunks[0][0] == b'VP8X':
        chunks[0] = (b'VP8X', bytes([chunks[0][1][0] | 8]) + chunks[0][1][1:])
    else:
        with Image.open(path) as image:
            width, height = image.size
        chunks.insert(0, (b'VP8X', bytes([8,0,0,0]) + (width-1).to_bytes(3,'little') + (height-1).to_bytes(3,'little')))
    exif = Image.Exif()
    exif[270] = json.dumps(metadata, sort_keys=True, separators=(',',':'))
    chunks.append((b'EXIF', exif.tobytes()))
    payload = b'WEBP' + b''.join(tag + struct.pack('<I', len(value)) + value + (b'\0' if len(value)&1 else b'') for tag,value in chunks)
    path.write_bytes(b'RIFF' + struct.pack('<I', len(payload)) + payload)

def validate(collection, destination):
    index = load(destination / 'gallery.json')
    manifest = load(destination / 'gallery-evidence.json')['games']
    if index.get('version') != 1 or set(index['games']) != set(manifest):
        raise ValueError('Gallery index and capture evidence differ')
    plan = {game['id']:game for game in load(destination/'gallery-captures.json')['games']}
    ids = {game['id'] for game in load(collection)['games']}
    if set(manifest) != ids:
        raise ValueError(f'Gallery/catalog IDs differ: missing {ids-set(manifest)}, extra {set(manifest)-ids}')
    count = total = 0
    for game_id, gallery in manifest.items():
        shots, captures = gallery['screenshots'], gallery['captures']
        if index['games'][game_id] != shots:
            raise ValueError(f'{game_id}: gallery paths differ from evidence')
        replay_hash = hashlib.sha256(json.dumps(plan[game_id]['actions'],sort_keys=True,separators=(',',':')).encode()).hexdigest()
        minimum = plan[game_id].get('minimumCaptures', 3)
        if minimum < 2 or (minimum < 3 and not plan[game_id].get('captureLimitation')):
            raise ValueError(f'{game_id}: a short gallery needs an explicit limitation')
        if len(shots) < minimum or len(shots) != len(captures):
            raise ValueError(f'{game_id}: insufficient distinct authored captures')
        seen = set()
        pixels = set()
        for name, capture in zip(shots, captures):
            path = destination / name
            if not path.resolve().is_relative_to(destination.resolve()):
                raise ValueError('Gallery path leaves its collection')
            if capture['replaySha256'] != replay_hash:
                raise ValueError(f'{game_id}: capture replay changed; regenerate this gallery')
            if not path.is_file() or capture['sha256'] != digest(path):
                raise ValueError(f'{game_id}: missing or changed image {name}')
            if path.stat().st_size > 150_000:
                raise ValueError(f'{name}: image exceeds 150KB')
            with Image.open(path) as image:
                if json.loads(image.getexif().get(270, '{}')) != origin_metadata(game_id, capture):
                    raise ValueError(f'{name}: missing or stale embedded capture origin')
                if image.width > 640 or image.height > 480 or image.width < 200 or image.height < 150:
                    raise ValueError(f'{name}: unexpected image size {image.size}')
                pixel_hash = hashlib.sha256(image.convert('RGB').tobytes()).hexdigest()
            if capture['sha256'] in seen or pixel_hash in pixels:
                raise ValueError(f'{game_id}: duplicate image or decoded pixels')
            seen.add(capture['sha256']); pixels.add(pixel_hash)
            count += 1; total += path.stat().st_size
    print(f'{len(ids)} games, {count} distinct real captures, {total:,} image bytes: PASS')

def contact_sheet(destination, output):
    manifest = load(destination / 'gallery-evidence.json')['games']
    rows = list(manifest.items())
    for batch in range(0, len(rows), 11):
        subset = rows[batch:batch+11]
        sheet = Image.new('RGB', (960, len(subset)*202), '#141c23')
        draw = ImageDraw.Draw(sheet)
        for row, (game_id, gallery) in enumerate(subset):
            draw.text((8,row*202+3), gallery.get('name',game_id), fill='white')
            for column, shot in enumerate(gallery['screenshots'][:3]):
                image = Image.open(destination/shot).convert('RGB')
                image.thumbnail((312,174))
                sheet.paste(image,(column*320+(320-image.width)//2,row*202+24))
        sheet.save(output/f'contact-{batch//11+1}.jpg', quality=85)

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--dist',type=Path,default=ROOT/'dist/lcom-arcade-web')
    parser.add_argument('--plan',type=Path,default=ROOT/'collections/arcade/gallery-captures.json')
    parser.add_argument('--output',type=Path,default=ROOT/'collections/arcade')
    parser.add_argument('--game',action='append',default=[],help='Exact name or collection ID; repeatable')
    parser.add_argument('--check',action='store_true')
    parser.add_argument('--contact-sheets',type=Path)
    args = parser.parse_args()
    if args.check:
        validate(args.dist/'collection.json',args.output)
    else:
        args.output.mkdir(parents=True,exist_ok=True)
        plan = load(args.plan)
        by_id = {game['id']:game for game in plan['games']}
        manifest_path = args.output/'gallery.json'
        evidence_path = args.output/'gallery-evidence.json'
        if evidence_path.exists():
            manifest = load(evidence_path)['games']
        else:
            manifest = load(manifest_path) if manifest_path.exists() else {}
        # A first capture run can migrate the initial rich draft index.
        if 'games' in manifest:
            manifest = manifest['games']
        with tempfile.TemporaryDirectory(prefix='arcade-galleries-') as temp:
            temporary = Path(temp)
            result = subprocess.run(['node',str(Path(__file__).with_suffix('.mjs')),str(args.dist.resolve()),str(args.plan.resolve()),str(temporary),*args.game],cwd=ROOT)
            reports = load(temporary/'capture-results.json')
            for report in reports:
                if report.get('error') or report['errors']:
                    print('FAILED', report['name'], report.get('error'), report['errors']); continue
                game = by_id[report['id']]
                screenshots, captures = [], []
                for record in report['captures']:
                    if game.get('publish') and record['slug'] not in game['publish']:
                        continue
                    source = Path(record['file'])
                    relative = Path('galleries')/report['id']/(record['slug']+'.webp')
                    target = args.output/relative
                    target.parent.mkdir(parents=True,exist_ok=True)
                    with Image.open(source) as image:
                        image = image.convert('RGB')
                        if record.get('crop'):
                            x,y,width,height=record['crop']
                            if x<0 or y<0 or x+width>image.width or y+height>image.height:
                                raise ValueError('Focus crop leaves the original framebuffer')
                            image=image.crop((x,y,x+width,y+height))
                        image.thumbnail((640,480),Image.Resampling.LANCZOS)
                        image.save(target,format='WEBP',quality=84,method=6)
                    screenshots.append(relative.as_posix())
                    captures.append({'source':'browser gameplay canvas','sha256':digest(target),'originalSha256':digest(source),'caption':record['caption'],'framebuffer':[record['width'],record['height']],'crop':record.get('crop'),'replay':f'gallery-captures.json#{report["id"]}','replaySha256':hashlib.sha256(json.dumps(game['actions'],sort_keys=True,separators=(',',':')).encode()).hexdigest(),'gameFiles':[{'file':Path(f['path']).name,'sha256':digest(Path(f['path']))} for f in report.get('gameFiles',[])]})
                for relative, capture in zip(screenshots, captures):
                    embed_origin(args.output/relative, origin_metadata(report['id'], capture))
                    capture['sha256'] = digest(args.output/relative)
                manifest[report['id']] = {'name':report['name'],'screenshots':screenshots,'captures':captures}
            evidence_path.write_text(json.dumps({'version':1,'games':manifest},indent=2)+'\n')
            manifest_path.write_text(json.dumps({'version':1,'games':{key:value['screenshots'] for key,value in manifest.items()}},indent=2)+'\n')
            if result.returncode:
                raise SystemExit(result.returncode)
    if args.contact_sheets:
        args.contact_sheets.mkdir(parents=True,exist_ok=True)
        contact_sheet(args.output,args.contact_sheets)

if __name__ == '__main__':
    main()
