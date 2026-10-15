import { defineConfig } from "vitepress";

const labSidebar = [
  {
    text: "Start",
    items: [
      { text: "Lab Track", link: "/labs/" },
    ],
  },
  {
    text: "Lab 1: RTC",
    items: [
      { text: "Introduction", link: "/labs/lab1/" },
      { text: "Why I/O Looked This Way", link: "/labs/lab1/context" },
      { text: "I/O Ports", link: "/labs/lab1/io-ports" },
      { text: "Bitwise Operations", link: "/labs/lab1/bitwise" },
      { text: "Reading The RTC", link: "/labs/lab1/rtc" },
      { text: "Implementation Tasks", link: "/labs/lab1/tasks" },
      { text: "Checks And Reading", link: "/labs/lab1/check" },
      { text: "How It Works Today", link: "/labs/lab1/modern" },
    ],
  },
  {
    text: "Lab 2: Timer",
    items: [
      { text: "Introduction", link: "/labs/lab2/" },
      { text: "Why PC Timers Exist", link: "/labs/lab2/context" },
      { text: "i8254 Controller", link: "/labs/lab2/timer-controller" },
      { text: "Programming Timers", link: "/labs/lab2/programming" },
      { text: "Interrupts", link: "/labs/lab2/interrupts" },
      { text: "Reusable Timer Lib", link: "/labs/lab2/library" },
      { text: "Implementation Tasks", link: "/labs/lab2/tasks" },
      { text: "Checks And Reading", link: "/labs/lab2/check" },
      { text: "How It Works Today", link: "/labs/lab2/modern" },
    ],
  },
  {
    text: "Lab 3: Keyboard",
    items: [
      { text: "Introduction", link: "/labs/lab3/" },
      { text: "The PC Keyboard", link: "/labs/lab3/context" },
      { text: "Keyboard Controller", link: "/labs/lab3/keyboard" },
      { text: "Reading Scancodes", link: "/labs/lab3/scancodes" },
      { text: "Interrupts vs Polling", link: "/labs/lab3/sync" },
      { text: "KBC Commands", link: "/labs/lab3/kbc-commands" },
      { text: "Multiple Devices", link: "/labs/lab3/multiple-devices" },
      { text: "Implementation Tasks", link: "/labs/lab3/tasks" },
      { text: "Checks And Reading", link: "/labs/lab3/check" },
      { text: "How It Works Today", link: "/labs/lab3/modern" },
    ],
  },
  {
    text: "Lab 4: Mouse",
    items: [
      { text: "Introduction", link: "/labs/lab4/" },
      { text: "Why Pointing Devices Matter", link: "/labs/lab4/context" },
      { text: "Mouse Controller", link: "/labs/lab4/mouse-controller" },
      { text: "Reading Packets", link: "/labs/lab4/packets" },
      { text: "Implementation Tasks", link: "/labs/lab4/tasks" },
      { text: "Checks And Reading", link: "/labs/lab4/check" },
      { text: "How It Works Today", link: "/labs/lab4/modern" },
    ],
  },
  {
    text: "Lab 5: Graphics",
    items: [
      { text: "Introduction", link: "/labs/lab5/" },
      { text: "Why Video Hardware Looks This Way", link: "/labs/lab5/context" },
      { text: "The Video Card", link: "/labs/lab5/video-card" },
      { text: "VBE", link: "/labs/lab5/vbe" },
      { text: "Video Memory", link: "/labs/lab5/vram" },
      { text: "Rectangles", link: "/labs/lab5/rectangles" },
      { text: "XPM Sprites", link: "/labs/lab5/xpm" },
      { text: "Reactive Graphics", link: "/labs/lab5/reactive-apps" },
      { text: "Implementation Tasks", link: "/labs/lab5/tasks" },
      { text: "Checks And Reading", link: "/labs/lab5/check" },
      { text: "How It Works Today", link: "/labs/lab5/modern" },
    ],
  },
  {
    text: "Lab 6: Audio",
    items: [
      { text: "Introduction", link: "/labs/lab6/" },
      { text: "PCM Audio", link: "/labs/lab6/pcm" },
      { text: "Implementation Tasks", link: "/labs/lab6/tasks" },
      { text: "Checks And Reading", link: "/labs/lab6/check" },
      { text: "How It Works Today", link: "/labs/lab6/modern" },
    ],
  },
  {
    text: "Lab 7: UART",
    items: [
      { text: "Introduction", link: "/labs/lab7/" },
      { text: "Serial Protocols", link: "/labs/lab7/protocols" },
      { text: "Implementation Tasks", link: "/labs/lab7/tasks" },
      { text: "Checks And Reading", link: "/labs/lab7/check" },
      { text: "How It Works Today", link: "/labs/lab7/modern" },
    ],
  },
];

const developerSidebar = [
  {
    text: "Developer Docs",
    items: [
      { text: "Maintainer Guide", link: "/developers/" },
      { text: "Architecture", link: "/developers/architecture" },
      { text: "Runtime And CLI", link: "/developers/runtime-cli" },
      { text: "Docs Site", link: "/developers/docs-site" },
      { text: "Releases", link: "/developers/releases" },
      { text: "From Minix", link: "/developers/from-minix" },
    ],
  },
];

const examplesSidebar = [
  { text: "Examples", items: [
    { text: "Overview", link: "/examples/" },
    { text: "Showcase Studios", link: "/examples/showcase-studios" },
  ] },
  { text: "Creative tools", items: [
    { text: "Notes", link: "/examples/word-processor" },
    { text: "Pocket Composer", link: "/examples/music-maker" },
    { text: "Pixel Studio", link: "/examples/pixel-studio" },
    { text: "Tile Forge", link: "/examples/tilemap-editor" },
  ] },
  { text: "Devices in action", items: [
    { text: "Focus Clock", link: "/examples/pomodoro-clock" },
    { text: "Interrupt Scope", link: "/examples/interrupt-scope" },
    { text: "Shared Whiteboard", link: "/examples/serial-whiteboard" },
    { text: "Serial Transfer", link: "/examples/serial-transfer" },
    { text: "Ambient Aquarium", link: "/examples/ambient-aquarium" },
  ] },
  { text: "Games", items: [
    { text: "Breakout", link: "/examples/breakout" },
    { text: "Flappy Bird", link: "/examples/flappy-bird" },
    { text: "Ninjix", link: "/examples/ninjix" },
    { text: "Plants vs. Zombies", link: "/examples/plants-vs-zombies" },
  ] },
  { text: "Contributing", items: [
    { text: "Update screenshots", link: "/examples/screenshots" },
  ] },
];

export default defineConfig({
  title: "lcom-ng",
  description: "Run LCOM labs and C projects without a Minix VM.",
  lang: "en-US",
  cleanUrls: true,
  sitemap: { hostname: "https://lcom.rgo.pt" },
  transformHead({ pageData }) {
    const path = pageData.relativePath.replace(/(^|\/)index\.md$/, "$1").replace(/\.md$/, "");
    const url = `https://lcom.rgo.pt/${path}`;
    return [["link", { rel: "canonical", href: url }], ["meta", { property: "og:url", content: url }]];
  },
  lastUpdated: true,
  head: [
    ["meta", { name: "theme-color", content: "#101820" }],
    ["meta", { property: "og:title", content: "lcom-ng" }],
    ["meta", { property: "og:description", content: "Run LCOM labs and C projects without a Minix VM." }],
    ["meta", { property: "og:type", content: "website" }],
  ],
  themeConfig: {
    siteTitle: "lcom-ng",
    search: {
      provider: "local",
    },
    nav: [
      { text: "Download", link: "/students/install" },
      { text: "Labs", link: "/labs/" },
      { text: "Adopt", link: "/instructors/adoption" },
      { text: "Examples", link: "/examples/" },
      { text: "Developers", link: "/developers/" },
      { text: "GitHub", link: "https://github.com/rodrgds/lcom-ng" },
    ],
    sidebar: {
      "/labs/": labSidebar,
      "/students/": [
        { text: "Students", items: [{ text: "Download and install", link: "/students/install" }, { text: "Existing Minix project", link: "/students/compatibility" }, { text: "Native labs", link: "/students/" }, { text: "Browser export", link: "/students/browser" }, { text: "macOS details", link: "/students/macos" }] },
      ],
      "/instructors/": [
        {
          text: "Instructors",
          items: [
            { text: "Course Guide", link: "/instructors/" },
            { text: "Adoption", link: "/instructors/adoption" },
            { text: "Syllabus", link: "/instructors/syllabus" },
            { text: "Project Rubric", link: "/instructors/rubric" },
          ],
        },
      ],
      "/developers/": [
        ...developerSidebar,
      ],
      "/examples/": [
        ...examplesSidebar,
      ],
    },
    socialLinks: [
      { icon: "github", link: "https://github.com/rodrgds/lcom-ng" },
    ],
    footer: {
      message: "Course/docs licensed CC BY 4.0. Code licensed MIT.",
      copyright: "lcom-ng contributors",
    },
  },
});
