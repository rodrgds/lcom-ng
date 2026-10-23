import {repositoryIcon} from './previews.js';

// Credits are authored in game-metadata.json: either a plain name or a
// {name, email, url} record when the original project publishes contact details.
function creditItem(author) {
  const item = document.createElement('li');
  item.className = 'load-credit';
  const name = document.createElement(author.url ? 'a' : 'span');
  name.textContent = author.name;
  if (author.url) {
    name.href = author.url;
    name.target = '_blank';
    name.rel = 'noopener noreferrer';
  }
  item.append(name);
  if (author.email) {
    const email = document.createElement('a');
    email.className = 'load-credit-email';
    email.href = 'mailto:' + author.email;
    email.textContent = author.email;
    item.append(email);
  }
  return item;
}

export function renderCredits(section, game) {
  const list = section.querySelector('ul');
  const authors = (game?.authors || []).map(author => typeof author === 'string' ? {name: author} : author)
    .filter(author => author && author.name);
  list.replaceChildren(...authors.map(creditItem));
  section.hidden = !authors.length;
  // The evidence that decided the credits stays available on the heading, so
  // person links keep their own tooltips.
  const heading = section.querySelector('h3');
  const detail = game?.creditsSource?.detail || '';
  if (detail) heading.title = detail;
  else heading.removeAttribute('title');
}

export function renderRepository(link, game) {
  const url = game?.repository || '';
  link.hidden = !url;
  if (!url) {
    link.removeAttribute('href');
    link.removeAttribute('title');
    link.replaceChildren();
    return;
  }
  link.href = url;
  link.title = url;
  link.setAttribute('aria-label', 'Source repository for ' + game.name);
  link.innerHTML = repositoryIcon(url);
  const label = document.createElement('span');
  label.textContent = url.replace(/^https?:\/\/(?:www\.)?/i, '').replace(/\/$/, '');
  link.append(label);
}
