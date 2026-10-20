# Docs site

The public website is built with VitePress and deployed to Cloudflare Pages.

Production URL: <https://lcom.rgo.pt/>

## Commands

```sh
bun install
bun run docs:dev
bun run docs:check
bun run deploy:docs
```

`docs:dev` and `docs:check` sync assets first. `docs:check` then builds the site
and runs `scripts/check_docs_site.sh`, which covers required pages, lab guide
shape, generated assets, and the built output.

`deploy:docs` builds and deploys with Wrangler from a configured Cloudflare
account. Production deployments normally run through GitHub Actions.

## Source layout

| Path | Purpose |
| --- | --- |
| `docs-site/index.md` | public landing page |
| `docs-site/labs/` | student lab guide chapters |
| `docs-site/students/` | student workflow |
| `docs-site/instructors/` | adoption, syllabus, rubric, game collections |
| `docs-site/developers/` | maintainer documentation |
| `docs-site/examples/` | demos and project seeds |
| `docs-site/.vitepress/config.mts` | navigation, sidebars, metadata |
| `docs-site/.vitepress/theme/` | site styling |

## Assets

Images and videos are source-controlled under `docs/assets/`. The sync script
copies public screenshots, gallery captures, and the demo video into the
generated `docs-site/public/assets/` directory:

```sh
bun run docs:assets
```

Do not edit files under `docs-site/public/assets/` directly. Add or replace the
source under `docs/assets/` instead.

## Cloudflare Pages

The public domain is `lcom.rgo.pt`. The Cloudflare Pages resource keeps the
internal project ID `machine-lab-docs` for deployments and old URLs. This is an
infrastructure identifier, not the product name.

`wrangler.toml` sets:

```toml
name = "machine-lab-docs"
pages_build_output_dir = "docs-site/.vitepress/dist"
```

Push-to-deploy is configured in `.github/workflows/docs.yml`. It needs:

- `CLOUDFLARE_ACCOUNT_ID`
- `CLOUDFLARE_API_TOKEN`

Use a token with Pages deployment permission and keep it in GitHub secrets. The
workflow can also run manually with `deploy=true`.

To connect the public domain, associate `lcom.rgo.pt` with the existing project's
Custom domains tab, then set its DNS CNAME to `machine-lab-docs.pages.dev`.
Adding DNS alone does not associate the domain with Pages. See
[Cloudflare's custom-domain guide](https://developers.cloudflare.com/pages/configuration/custom-domains/).

The VitePress config generates canonical page URLs and a sitemap under
`https://lcom.rgo.pt`. `CLOUDFLARE_PAGES_PROJECT` may override the project ID as
a repository variable if the infrastructure is moved later.
