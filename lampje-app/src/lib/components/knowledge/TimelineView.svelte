<script lang="ts">
  import type { TimelineEntry as TimelineEntryType } from '$lib/types';
  import TimelineEntry from './TimelineEntry.svelte';
  import EmptyState from '$lib/components/ui/EmptyState.svelte';
  import { Clock } from 'lucide-svelte';

  let {
    entries
  }: {
    entries: TimelineEntryType[];
  } = $props();

  // Group entries by date string
  const groupedEntries = $derived.by(() => {
    const groups: { label: string; entries: TimelineEntryType[] }[] = [];
    const groupMap = new Map<string, TimelineEntryType[]>();

    // Sort entries by date descending
    const sorted = [...entries].sort((a, b) => b.date.getTime() - a.date.getTime());

    for (const entry of sorted) {
      const dateKey = entry.date.toLocaleDateString('nl-NL', {
        day: 'numeric',
        month: 'long',
        year: 'numeric'
      });
      if (!groupMap.has(dateKey)) {
        groupMap.set(dateKey, []);
      }
      groupMap.get(dateKey)!.push(entry);
    }

    for (const [label, groupEntries] of groupMap) {
      groups.push({ label, entries: groupEntries });
    }

    return groups;
  });
</script>

{#if entries.length > 0}
  <div class="relative pl-6">
    <!-- Vertical line -->
    <div class="absolute bottom-0 left-[5px] top-0 w-0.5 rounded-full bg-surface-2"></div>

    {#each groupedEntries as group, gi}
      <h4
        class="mb-3 font-label text-xs uppercase tracking-wider text-on-bg-muted {gi === 0 ? '' : 'mt-6'}"
      >
        {group.label}
      </h4>
      <div class="space-y-4">
        {#each group.entries as entry, ei (entry.id)}
          <TimelineEntry {entry} isFirst={gi === 0 && ei === 0} />
        {/each}
      </div>
    {/each}
  </div>
{:else}
  <EmptyState
    icon={Clock}
    title="Geen tijdlijn items"
    description="Er zijn nog geen gesprekken om weer te geven"
  />
{/if}
