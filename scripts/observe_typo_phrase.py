"""QA-only screenshot matcher for the original ten visible gold phrases."""
from pathlib import Path
import argparse,json,re,sys
from PIL import Image,ImageFilter,ImageChops
ROOT=Path(sys.argv[sys.argv.index('--source-directory')+1]) if '--source-directory' in sys.argv else Path(__file__).resolve().parents[1]/'examples/community/Xouteiro--TypeRacer-in-C/proj/src'
def letter(ch):
 text=(ROOT/'xpm/letters.xpm').read_text();m=re.search(r'\bxpm_'+ch+r'\s*\[\]\s*=\s*\{',text);rows=re.findall(r'"([^"\n]*)"',text[m.end():text.index('};',m.end())]);w,h,n,cpp=map(int,rows[0].split());palette={line[:cpp]:255 if line[cpp:].split('c ',1)[1].strip().lower()=='#ffbf00' else 0 for line in rows[1:n+1]};im=Image.new('L',(w,h));im.putdata([palette[row[x:x+cpp]] for row in rows[n+1:n+h+1] for x in range(0,w*cpp,cpp)]);return im
LETTERS={c:letter(c) for c in 'abcdefghijklmnopqrstuvwxyz'}
def template(phrase):
 out=Image.new('L',(800,600));x=safe=60;y=170
 for c in phrase:
  if (safe>=610 and c==' ') or safe>=750:y+=30;x=safe=40
  if c in LETTERS:
   im=LETTERS[c];layer=Image.new('L',out.size);layer.paste(im,(x,y));out=ImageChops.lighter(out,layer)
  elif c==' ':safe+=1;x+=1
  else:break
  safe+=18;x+=18
 return out
PHRASES=(ROOT/'xpm/phrases.txt').read_text().splitlines()
TEMPLATES=[template(p) for p in PHRASES]
def observe(path,rect=None):
 im=Image.open(path).convert('RGB')
 if rect is None and im.size != (800,600):
  blue=Image.new('L',im.size);blue.putdata([255 if r<=5 and abs(g-71)<=5 and abs(b-193)<=5 else 0 for r,g,b in im.getdata()]);box=blue.getbbox()
  if box is None or abs((box[2]-box[0])/(box[3]-box[1])-4/3)>.04:raise ValueError('Original blue guest rectangle not confidently visible')
  rect=[box[0],box[1],box[2]-box[0],box[3]-box[1]]
 if rect:
  x,y,w,h=rect;im=im.crop((x,y,x+w,y+h))
 im=im.resize((800,600),Image.Resampling.NEAREST);mask=Image.new('L',im.size);mask.putdata([255 if r>225 and 135<g<220 and b<70 else 0 for r,g,b in im.getdata()]);scope=(30,160,780,400);mask=mask.crop(scope);expanded=mask.filter(ImageFilter.MaxFilter(3));scores=[]
 for i,t in enumerate(TEMPLATES):
  t=t.crop(scope);tbig=t.filter(ImageFilter.MaxFilter(3));expected=sum(v!=0 for v in t.getdata());actual=sum(v!=0 for v in mask.getdata());hit=sum(v!=0 for v in ImageChops.multiply(t,expanded).getdata());reverse=sum(v!=0 for v in ImageChops.multiply(mask,tbig).getdata());recall=hit/max(expected,1);precision=reverse/max(actual,1);score=2*recall*precision/max(recall+precision,1e-9);scores.append({'index':i,'phrase':PHRASES[i],'score':score,'recall':recall,'precision':precision})
 scores.sort(key=lambda s:s['score'],reverse=True);best=scores[0];return {'gameRect':rect,'accepted':best['score']>=.86 and best['score']-scores[1]['score']>=.05,'best':best,'runnerUp':scores[1],'observedGoldPixels':sum(v!=0 for v in mask.getdata())}
if __name__=='__main__':
 ap=argparse.ArgumentParser();ap.add_argument('image');ap.add_argument('--source-directory',type=Path);ap.add_argument('--game-rect',type=int,nargs=4);a=ap.parse_args()
 print(json.dumps(observe(a.image,a.game_rect),indent=2))
