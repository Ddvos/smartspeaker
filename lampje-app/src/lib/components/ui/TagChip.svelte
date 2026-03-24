<script lang="ts">
  import { X } from 'lucide-svelte';

  let {
    label,
    color,
    removable = false,
    active = false,
    onclick,
    onremove
  }: {
    label: string;
    color?: string;
    removable?: boolean;
    active?: boolean;
    onclick?: () => void;
    onremove?: () => void;
  } = $props();

  const baseClasses = $derived(
    `inline-flex items-center gap-1.5 rounded-full px-3 py-1 font-label text-xs transition-colors duration-150 ${
      active ? 'bg-primary/15 text-primary' : 'bg-surface-2 text-on-bg-dim'
    }`
  );
</script>

{#snippet chipContent()}
  {#if color}
    <span
      class="inline-block h-1.5 w-1.5 shrink-0 rounded-full"
      style="background-color: {color};"
    ></span>
  {/if}

  {label}

  {#if removable}
    <button
      type="button"
      class="ml-0.5 shrink-0 cursor-pointer rounded-full p-0.5 text-on-bg-muted transition-colors duration-150 hover:text-on-bg-dim"
      onclick={(e: MouseEvent) => {
        e.stopPropagation();
        onremove?.();
      }}
      aria-label="Verwijder {label}"
    >
      <X size={12} />
    </button>
  {/if}
{/snippet}

{#if onclick}
  <button
    type="button"
    class="{baseClasses} cursor-pointer hover:bg-surface-bright"
    onclick={onclick}
  >
    {@render chipContent()}
  </button>
{:else}
  <span class={baseClasses}>
    {@render chipContent()}
  </span>
{/if}
