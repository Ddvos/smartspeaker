<script lang="ts">
  import TagChip from '$lib/components/ui/TagChip.svelte';
  import { conversationsStore } from '$lib/stores/conversations.svelte';

  const filters = [
    { key: 'all', label: 'Alle' },
    { key: 'today', label: 'Vandaag' },
    { key: 'week', label: 'Deze week' },
    { key: 'favorites', label: 'Favorieten' }
  ];
</script>

<div class="flex flex-wrap gap-2">
  {#each filters as filter}
    <TagChip
      label={filter.label}
      active={conversationsStore.activeFilter === filter.key}
      onclick={() => conversationsStore.setFilter(filter.key)}
    />
  {/each}
</div>
