import React from 'react';
import {
  AbsoluteFill, Audio, Easing, Img, OffthreadVideo, Sequence,
  interpolate, staticFile, useCurrentFrame,
} from 'remotion';

const clamp = {extrapolateLeft: 'clamp' as const, extrapolateRight: 'clamp' as const};
const ease = Easing.bezier(0.16, 1, 0.3, 1);
const ink = '#172731';
const light = '#edf3ee';

// Cuts follow actions in the actual replays, rather than a feature-card slideshow.
export const CUTS = [0, 120, 210, 330, 450, 570, 720, 810, 900, 945, 990, 1035, 1080, 1200];

const Footage: React.FC<{
  name: string; start?: number; width: number; height?: number;
}> = ({name, start = 0, width, height = width * 0.75}) => {
  const frame = useCurrentFrame();
  return <div style={{width, height, overflow: 'hidden', borderRadius: 8,
    boxShadow: '0 18px 50px #00000020',
    translate: `${interpolate(frame, [0, 20], [32, 0], {...clamp, easing: ease})}px 0`}}>
    <OffthreadVideo muted src={staticFile(`clips/${name}.mp4`)} startFrom={start}
      style={{width: '100%', height: '100%', objectFit: 'contain'}} />
  </div>;
};

const Title: React.FC<{children: React.ReactNode; size?: number}> = ({children, size = 88}) =>
  <div style={{fontWeight: 700, fontSize: size, lineHeight: 1.06, letterSpacing: '-0.035em', textWrap: 'balance'}}>{children}</div>;

const SideScene: React.FC<{
  title: React.ReactNode; detail: string; clip: string; start?: number; background: string;
  dark?: boolean;
}> = ({title, detail, clip, start, background, dark = false}) => {
  const frame = useCurrentFrame();
  return <AbsoluteFill style={{background, color: dark ? light : ink, padding: '60px 64px',
    display: 'flex', flexDirection: 'row', alignItems: 'center', gap: 42}}>
    <div style={{width: 510, flexShrink: 0, display: 'flex', flexDirection: 'column', gap: 38,
      translate: `0 ${interpolate(frame, [0, 24], [20, 0], {...clamp, easing: ease})}px`}}>
      <Title>{title}</Title>
      <div style={{fontSize: 30, lineHeight: 1.45, maxWidth: 450, color: dark ? '#b5c7cf' : '#4d626a'}}>{detail}</div>
    </div>
    <Footage name={clip} start={start} width={1216} />
  </AbsoluteFill>;
};

const WideScene: React.FC<{
  title: string; clip: string; start?: number; background: string; dark?: boolean;
}> = ({title, clip, start, background, dark}) => <AbsoluteFill style={{background,
  color: dark ? light : ink, alignItems: 'center', gap: 20, paddingTop: 26}}>
  <Title size={70}>{title}</Title>
  <Footage name={clip} start={start} width={1232} />
</AbsoluteFill>;

// This is a real paired-runtime recording, sampled from both presented surfaces.
const PairScene = () => <AbsoluteFill style={{background: '#e8eeec', color: ink,
  padding: '56px 64px', alignItems: 'center', justifyContent: 'center', gap: 38}}>
  <Title size={80}>A connection you can see.</Title>
  <Footage name="serial_whiteboard" start={18} width={1792} height={1792 * 600 / 1632} />
  <div style={{fontSize: 34, color: '#4d626a'}}>Two C programs. One virtual serial cable.</div>
</AbsoluteFill>;

const Opening = () => <AbsoluteFill style={{background: '#122821', color: '#eff5e9',
  display: 'flex', flexDirection: 'row', alignItems: 'center', padding: 64, gap: 48}}>
  <div style={{width: 520, flexShrink: 0, display: 'flex', flexDirection: 'column', gap: 44}}>
    <div style={{fontSize: 38, fontWeight: 700}}>lcom-ng</div>
    <Title size={104}>Make C<br />come alive.</Title>
    <div style={{fontSize: 30, color: '#c3d4c4'}}>Start with a program.<br />See what it can become.</div>
  </div>
  <Footage name="ninjix" start={300} width={1216} />
</AbsoluteFill>;

const GameScene: React.FC<{title: React.ReactNode; clip: string; start: number}> =
  ({title, clip, start}) => <AbsoluteFill style={{background: '#101922', color: light,
    padding: '32px 64px', flexDirection: 'row', alignItems: 'center', gap: 42}}>
    <div style={{width: 320, flexShrink: 0}}><Title size={74}>{title}</Title></div>
    <Footage name={clip} start={start} width={1350} />
  </AbsoluteFill>;

const Games = () => <AbsoluteFill>
  <Sequence premountFor={15} from={0} durationInFrames={45}>
    <GameScene title="Breakout" clip="breakout" start={15} />
  </Sequence>
  <Sequence premountFor={15} from={45} durationInFrames={45}>
    <GameScene title="Flappy" clip="flappy" start={138} />
  </Sequence>
  <Sequence premountFor={15} from={90} durationInFrames={45}>
    <GameScene title="Ninjix" clip="ninjix" start={450} />
  </Sequence>
  <Sequence premountFor={15} from={135} durationInFrames={45}>
    <GameScene title={<>Plants vs.<br />Zombies</>} clip="pvz" start={480} />
  </Sequence>
</AbsoluteFill>;

const Closing = () => {
  const frame = useCurrentFrame();
  const images = ['pixel-studio', 'music-maker', 'tilemap-editor', 'word-processor', 'ambient-aquarium', 'interrupt-scope', 'pomodoro-clock', 'serial-whiteboard-left', 'serial-transfer-sender'];
  return <AbsoluteFill style={{background: '#eef1eb', color: ink, display: 'flex',
    flexDirection: 'row', alignItems: 'center', padding: '72px 88px', gap: 80}}>
    <div style={{width: 870, display: 'flex', flexDirection: 'column', gap: 34}}>
      <Title size={126}>lcom-ng</Title>
      <div style={{fontSize: 46, lineHeight: 1.2}}>C programs. Real feedback.</div>
      <div style={{fontSize: 28, color: '#4d626a', marginTop: 16}}>Build with virtual devices.<br />Run on macOS, Linux and Windows.</div>
      <div style={{fontSize: 42, fontWeight: 700, color: '#246b5d', marginTop: 40}}>lcom.rgo.pt</div>
    </div>
    <div style={{width: 780, display: 'grid', gridTemplateColumns: '1fr 1fr 1fr', gap: 20,
      translate: `0 ${interpolate(frame, [0, 120], [24, -12], clamp)}px`}}>
      {images.map((name) => <Img key={name} src={staticFile(`screenshots/${name}.png`)}
        style={{width: '100%', borderRadius: 5, boxShadow: '0 12px 24px #00000014'}} />)}
    </div>
  </AbsoluteFill>;
};

export const MachineLabAnnouncement = () => <AbsoluteFill style={{fontFamily: 'Launch Sans', background: '#122821'}}>
  <style>{`
    @font-face {font-family: 'Launch Sans'; src: url('${staticFile('fonts/LaunchSans.ttf')}'); font-weight: 400;}
    @font-face {font-family: 'Launch Sans'; src: url('${staticFile('fonts/LaunchSans-Bold.ttf')}'); font-weight: 700;}
  `}</style>
  <Audio src={staticFile('audio/ambient.wav')} volume={0.72} />
  <Sequence premountFor={15} from={0} durationInFrames={120}><Opening /></Sequence>
  <Sequence premountFor={15} from={120} durationInFrames={90}>
    <WideScene title="Write. Save. Keep going." clip="word-processor" background="#e8eeec" />
  </Sequence>
  <Sequence premountFor={15} from={210} durationInFrames={120}>
    <SideScene title={<>Every pixel,<br />yours.</>} detail="Draw, fill, undo. Export a sprite you can use in C."
      clip="pixel_studio" start={36} background="#f0ede6" />
  </Sequence>
  <Sequence premountFor={15} from={330} durationInFrames={120}>
    <WideScene title="Give an idea a little rhythm." clip="music-maker" start={12} background="#141d28" dark />
  </Sequence>
  <Sequence premountFor={15} from={450} durationInFrames={120}>
    <SideScene title={<>Make a world.<br />Then play it.</>} detail="Paint a map. Check it. Step inside your own puzzle."
      clip="tilemap_editor" start={204} background="#e7ede8" />
  </Sequence>
  <Sequence premountFor={15} from={570} durationInFrames={150}><PairScene /></Sequence>
  <Sequence premountFor={15} from={720} durationInFrames={90}>
    <SideScene title="Take a breath." detail="A tiny habitat that follows the clock."
      clip="ambient_aquarium" start={30} background="#132d3b" dark />
  </Sequence>
  <Sequence premountFor={15} from={810} durationInFrames={90}>
    <WideScene title="See what makes it tick." clip="interrupt_scope" start={12} background="#141d28" dark />
  </Sequence>
  <Sequence premountFor={15} from={900} durationInFrames={180}><Games /></Sequence>
  <Sequence premountFor={15} from={1080} durationInFrames={120}><Closing /></Sequence>
</AbsoluteFill>;
