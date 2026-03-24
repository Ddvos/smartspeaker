<script lang="ts">
  import type { TimelineEntry, Mood } from '$lib/types';
  import TagChip from '$lib/components/ui/TagChip.svelte';
  import { formatDate } from '$lib/utils/formatters';

  let {
    entry,
    isFirst = false
  }: {
    entry: TimelineEntry;
    isFirst?: boolean;
  } = $props();

  const moodColors: Record<Mood, string> = {
    focused: '#81ecff',
    happy: '#ff7520',
    curious: '#a68cff',
    playful: '#ff716c',
    neutral: '#6b7280'
  };

  const dotColor = $derived(moodColors[entry.mood] ?? '#6b7280');
</script>

<div class="relative">
  <!-- Dot -->
  <div
    class="absolute left-0 top-1 h-3 w-3 rounded-full"
    style="background-color: {dotColor};"
  ></div>

  <!-- Content -->
  <div class="ml-8">
    <p class="font-body text-sm font-medium text-on-bg">{entry.title}</p>
    {#if entry.tags.length > 0}
      <div class="mt-1 flex gap-1.5">
        {#each entry.tags as tag}
          <TagChip label={tag} />
        {/each}
      </div>
    {/if}
    <p class="mt-1 font-label text-xs text-on-bg-muted">{formatDate(entry.date)}</p>
  </div>
</div>
