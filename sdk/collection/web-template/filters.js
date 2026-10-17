import {cue} from './audio.js';

export class GameFilters {
  constructor(catalog,taxonomy,change) {
    this.catalog = catalog;
    this.taxonomy = taxonomy;
    this.change = change;
    this.selected = {genre:new Set(),players:new Set(),year:new Set()};
    this.input = document.querySelector('#search');
    this.query = '';
    this.storageKey = 'lcom-arcade:browse:v1:' + location.pathname;
    try {
      const saved = JSON.parse(sessionStorage.getItem(this.storageKey));
      if (saved && typeof saved.query === 'string') this.query = saved.query;
      for (const facet of Object.keys(this.selected)) {
        if (Array.isArray(saved?.[facet])) this.selected[facet] = new Set(saved[facet].filter(value =>
          typeof value === 'string' && (facet === 'year' ? catalog.some(game=>game.academicYear===value) : taxonomy.some(tag=>tag.facet===facet&&tag.id===value))));
      }
    } catch { /* Browsing works when session storage is unavailable. */ }
    this.input.value = this.query;
    this.input.oninput = () => {this.query=this.input.value;this.apply();};
    this.input.form.onsubmit = event => {event.preventDefault();this.close(false);document.querySelector('#games .game-launch')?.focus();};
    this.panel = document.querySelector('#browse-panel');
    this.toggle = document.querySelector('#browse-toggle');
    this.toggle.addEventListener('click',() => this.open());
    this.panel.querySelector('[data-dismiss]').onclick = () => this.close();
    this.panel.querySelector('[data-reset]').onclick = () => this.reset();
    document.addEventListener('pointerdown',event => {
      if (!this.panel.hidden && !this.panel.contains(event.target) && !this.toggle.contains(event.target)) this.close(false);
    });
    document.addEventListener('keydown',event => {
      if (!this.panel.hidden && event.key === 'Escape') {event.preventDefault();event.stopPropagation();this.close();}
    });
    const groups = [
      ['genre','Genre',taxonomy.filter(tag=>tag.facet==='genre').map(tag=>({...tag,value:tag.id}))],
      ['players','Players',taxonomy.filter(tag=>tag.facet==='players').map(tag=>({...tag,value:tag.id}))],
      ['year','School year',[...new Set(catalog.map(game=>game.academicYear).filter(Boolean))].sort().reverse().map(year=>({value:year,label:year}))]
    ];
    this.buttons = [];
    for(const [facet,label,options] of groups) {
      const fieldset=document.createElement('fieldset'),legend=document.createElement('legend'),grid=document.createElement('div');
      legend.textContent=label;grid.className='filter-options';fieldset.append(legend,grid);
      for(const option of options) {
        const total=catalog.filter(game=>facet==='year'?game.academicYear===option.value:game.tags?.includes(option.value)).length;
        if(!total)continue;
        const button=document.createElement('button'),text=document.createElement('span'),number=document.createElement('span');
        button.type='button';button.setAttribute('aria-pressed','false');button.dataset.facet=facet;button.dataset.value=option.value;
        if(option.description)button.title=option.description;
        text.textContent=option.label;number.textContent=total;number.setAttribute('aria-hidden','true');button.append(text,number);
        button.onclick=()=>{const values=this.selected[facet];if(values.has(option.value))values.delete(option.value);else values.add(option.value);this.apply();cue('select',{emphasis:'subtle'});};
        grid.append(button);this.buttons.push(button);
      }
      if(grid.children.length)this.panel.querySelector('.filter-groups').append(fieldset);
    }
    window.addEventListener('resize',()=>{if(!this.panel.hidden)this.position();});

    this.apply(false);
  }
  matches(game,except=null) {
    const labels=this.taxonomy.filter(tag=>game.tags?.includes(tag.id)).map(tag=>tag.label);
    const text=[game.name,game.academicYear,...labels].filter(Boolean).join(' ').toLocaleLowerCase();
    return text.includes(this.query.trim().toLocaleLowerCase()) && Object.entries(this.selected).every(([facet,values])=>facet===except || !values.size ||
      (facet==='year'?values.has(game.academicYear):[...values].some(value=>game.tags?.includes(value))));
  }
  apply(notify=true) {
    const matches=this.catalog.filter(game=>this.matches(game));
    for(const button of this.buttons) {
      const {facet,value}=button.dataset,selected=this.selected[facet].has(value);
      button.setAttribute('aria-pressed',String(selected));
      const count=this.catalog.filter(game=>this.matches(game,facet)&&(facet==='year'?game.academicYear===value:game.tags?.includes(value))).length;
      button.lastElementChild.textContent=count;button.disabled=!selected&&!count;
    }
    const selected=Object.values(this.selected).reduce((sum,values)=>sum+values.size,0);
    const active=!!selected || !!this.query.trim();
    document.querySelector('#browse-count').textContent=`${matches.length} ${matches.length===1?'game':'games'}`;
    this.toggle.classList.toggle('filtered',!!selected);
    this.toggle.querySelector('span').textContent=selected?`Filters (${selected})`:'Filters';
    this.panel.querySelector('[data-reset]').disabled=!active;
    const chips=document.querySelector('#active-filters');chips.replaceChildren();chips.hidden=!active;
    const chip=(label,remove)=>{const button=document.createElement('button');button.type='button';button.textContent=label;button.setAttribute('aria-label','Remove '+label);button.onclick=remove;chips.append(button);};
    if(this.query.trim())chip('Search: '+this.query.trim(),()=>{this.query='';this.input.value='';this.apply();this.input.focus();});
    for(const [facet,values] of Object.entries(this.selected))for(const value of values)chip(facet==='year'?value:this.taxonomy.find(tag=>tag.id===value)?.label||value,()=>{values.delete(value);this.apply();this.input.focus();});
    if(active){const clear=document.createElement('button');clear.type='button';clear.className='clear-browse';clear.textContent='Clear all';clear.setAttribute('aria-label','Clear search and filters');clear.onclick=()=>{this.reset();this.input.focus();};chips.append(clear);}
    try {sessionStorage.setItem(this.storageKey,JSON.stringify({query:this.query,...Object.fromEntries(Object.entries(this.selected).map(([facet,values])=>[facet,[...values]]))}));} catch { /* Optional persistence. */ }
    this.panel.querySelector('[role="status"]').textContent=`${matches.length} of ${this.catalog.length} games`;
    if(notify)this.change(matches);
  }
  reset() {
    this.query='';this.input.value='';for(const values of Object.values(this.selected))values.clear();this.apply();
  }
  position() {
    const button=this.toggle.getBoundingClientRect(),panel=this.panel.getBoundingClientRect();
    this.panel.style.left=Math.max(8,Math.min(innerWidth-panel.width-8,button.left))+'px';
    this.panel.style.top=Math.max(8,Math.min(innerHeight-panel.height-8,button.bottom+8))+'px';
  }
  open() {
    if(!this.panel.hidden){this.close();return;}
    this.panel.hidden=false;this.toggle.setAttribute('aria-expanded','true');this.position();
    if(!matchMedia('(prefers-reduced-motion: reduce)').matches)this.panel.animate([{transform:'translateY(-8px)'},{transform:'none'}],{duration:220,easing:'cubic-bezier(.16,1,.3,1)'});
    this.panel.querySelector('button:not(:disabled)')?.focus({preventScroll:true});
    cue('open',{emphasis:'subtle'});
  }
  close(focus=true) {
    if(this.panel.hidden)return;
    this.panel.hidden=true;this.toggle.setAttribute('aria-expanded','false');
    if(focus)this.toggle.focus({preventScroll:true});
  }
}
