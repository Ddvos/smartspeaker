<script lang="ts">
  import { Search } from 'lucide-svelte';

  let {
    value = $bindable(''),
    placeholder = 'Zoeken...',
    showShortcut = true,
    size = 'default',
    oninput
  }: {
    value?: string;
    placeholder?: string;
    showShortcut?: boolean;
    size?: 'default' | 'large';
    oninput?: (value: string) => void;
  } = $props();

  let focused = $state(false);

  function handleInput(e: Event) {
    const target = e.target as HTMLInputElement;
    value = target.value;
    oninput?.(value);
  }
</script>

<div
  class="flex items-center bg-surface-1 rounded-xl px-4 transition-all duration-150 {size === 'large' ? 'py-4' : 'py-3'} {focused ? 'ring-2 ring-primary/20 shadow-glow-sm' : ''}"
>
  <Search size={18} class="text-on-bg-muted shrink-0" />

  <input
    type="text"
    class="flex-1 bg-transparent text-on-bg font-body outline-none placeholder:text-on-bg-muted ml-3 {size === 'large' ? 'text-base' : 'text-sm'}"
    {placeholder}
    bind:value
    oninput={handleInput}
    onfocus={() => (focused = true)}
    onblur={() => (focused = false)}
  />

  {#if showShortcut && !value}
    <kbd class="ml-2 shrink-0 rounded-lg bg-surface-2 px-2 py-0.5 font-label text-xs text-on-bg-muted">
      ⌘K
    </kbd>
  {/if}
</div>
