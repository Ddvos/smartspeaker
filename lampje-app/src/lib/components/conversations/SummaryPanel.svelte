<script lang="ts">
  import { ChevronDown } from 'lucide-svelte';
  import TagChip from '$lib/components/ui/TagChip.svelte';

  let {
    summary,
    tags
  }: {
    summary: string;
    tags: string[];
  } = $props();

  let isOpen = $state(true);
</script>

<div class="rounded-xl bg-surface-1 p-5">
  <button
    type="button"
    class="flex w-full cursor-pointer items-center justify-between"
    onclick={() => (isOpen = !isOpen)}
  >
    <span class="font-display text-sm text-on-bg-dim">Samenvatting</span>
    <div class="transition-transform duration-200 {isOpen ? 'rotate-180' : 'rotate-0'}">
      <ChevronDown size={16} class="text-on-bg-muted" />
    </div>
  </button>

  {#if isOpen}
    <div class="mt-3">
      <p class="font-body text-sm leading-relaxed text-on-bg-dim">{summary}</p>
      {#if tags.length > 0}
        <div class="mt-3 flex flex-wrap gap-2">
          {#each tags as tag}
            <TagChip label={tag} />
          {/each}
        </div>
      {/if}
    </div>
  {/if}
</div>
