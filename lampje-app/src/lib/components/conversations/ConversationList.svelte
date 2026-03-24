<script lang="ts">
  import { MessageSquare } from 'lucide-svelte';
  import SearchBar from '$lib/components/ui/SearchBar.svelte';
  import EmptyState from '$lib/components/ui/EmptyState.svelte';
  import FilterChips from './FilterChips.svelte';
  import ConversationItem from './ConversationItem.svelte';
  import { conversationsStore } from '$lib/stores/conversations.svelte';
</script>

<div class="flex flex-col gap-3">
  <SearchBar
    value={conversationsStore.searchTerm}
    oninput={(val) => conversationsStore.setSearch(val)}
    placeholder="Zoeken in gesprekken..."
  />

  <FilterChips />
</div>

<div class="mt-4 flex-1 space-y-1 overflow-y-auto">
  {#if conversationsStore.filtered.length > 0}
    {#each conversationsStore.filtered as conversation (conversation.id)}
      <ConversationItem
        {conversation}
        isSelected={conversationsStore.activeId === conversation.id}
        onclick={() => conversationsStore.setActive(conversation.id)}
      />
    {/each}
  {:else}
    <EmptyState
      icon={MessageSquare}
      title="Geen gesprekken gevonden"
    />
  {/if}
</div>
