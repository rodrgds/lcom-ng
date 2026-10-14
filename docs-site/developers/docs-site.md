# Docs Site

The public website is built with VitePress and deployed to Cloudflare Pages.

Production URL: <https://lcom.rgo.pt/>

## Commands

```sh
bun install
bun run docs:dev
bun run docs:check
bun run deploy:docs
```

`docs:check` builds the site and runs `scripts/check_docs_site.sh`.

## Source Layout

| Path | Purpose |
| --- | --- |
| `docs-site/index.md` | public landing page |
| `docs-site/labs/` | student lab guide chapters |
| `docs-site/students/` | student workflow |
| `docs-site/instructors/` | adoption, syllabus, rubric |
| `docs-site/developers/` | maintainer documentation |
| `docs-site/examples/` | demos and project seeds |
| `docs-site/.vitepress/config.mts` | navigation, sidebars, metadata |
| `docs-site/.vitepress/theme/` | site styling |

## Assets

Images and videos are source-controlled under `docs/assets/`. The site build
copies them into `docs-site/public/assets/`:

```sh
bun run docs:assets
```

Do not edit generated files under `docs-site/public/assets/` directly.

## Cloudflare Pages

The canonical public domain is `lcom.rgo.pt`. The existing Cloudflare Pages resource
keeps the internal project ID `machine-lab-docs` so deployments and old URLs continue
to work. This is an infrastructure identifier, not the public product name.

`wrangler.toml` sets:

```toml
name = "machine-lab-docs"
pages_build_output_dir = "docs-site/.vitepress/dist"
```

Push-to-deploy is configured in `.github/workflows/docs.yml`. It needs:

- `CLOUDFLARE_ACCOUNT_ID`
- `CLOUDFLARE_API_TOKEN`

Use a token with Pages deployment permission. Keep its value in GitHub secrets.

To connect the public domain, associate `lcom.rgo.pt` with the existing project's
Custom domains tab, then set its DNS CNAME to `machine-lab-docs.pages.dev`.
Adding DNS alone does not associate the domain with Pages. See
[Cloudflare's custom-domain guide](https://developers.cloudflare.com/pages/configuration/custom-domains/).

The VitePress config generates canonical page URLs and a sitemap under `https://lcom.rgo.pt`.
Both `workflow_dispatch` with `deploy=true` and a main-branch push can deploy the
verified build. `CLOUDFLARE_PAGES_PROJECT` may override the existing project ID as a
repository variable if the infrastructure is intentionally moved later.
