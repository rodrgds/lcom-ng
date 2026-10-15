import DefaultTheme from "vitepress/theme";
import ExampleGallery from "./components/ExampleGallery.vue";
import "./custom.css";

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component("ExampleGallery", ExampleGallery);
  },
};
