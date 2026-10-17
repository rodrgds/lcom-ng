// Category labels are informational, leaving game actions in the normal Tab order.
export function renderGameTags(element, game, taxonomy) {
  const labels = new Map(taxonomy.map(tag => [tag.id, tag.label]));
  const items = [];
  for (const id of game.tags || []) {
    const label = labels.get(id);
    if (!label) continue;
    const item = element.ownerDocument.createElement('li');
    item.textContent = label;
    items.push(item);
  }
  element.replaceChildren(...items);
  element.hidden = !items.length;
}
