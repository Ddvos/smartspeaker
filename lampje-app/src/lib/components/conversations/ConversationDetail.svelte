<script lang="ts">
  import { MessageSquare } from 'lucide-svelte';
  import EmptyState from '$lib/components/ui/EmptyState.svelte';
  import TagChip from '$lib/components/ui/TagChip.svelte';
  import ChatMessage from './ChatMessage.svelte';
  import SummaryPanel from './SummaryPanel.svelte';
  import { conversationsStore } from '$lib/stores/conversations.svelte';
  import { formatDate, formatDuration } from '$lib/utils/formatters';
</script>

{#if conversationsStore.active}
  {@const c = conversationsStore.active}
  <div class="flex h-full flex-col">
    <!-- Header -->
    <div class="shrink-0">
      <h2 class="font-display text-xl text-on-bg">{c.title}</h2>
      <p class="mt-1 font-label text-xs text-on-bg-muted">
        {formatDate(c.createdAt)} &middot; {formatDuration(c.duration)} &middot; {c.messages.length} berichten
      </p>
      {#if c.tags.length > 0}
        <div class="mt-3 flex gap-2">
          {#each c.tags as tag}
            <TagChip
              label={tag}
              removable
              onremove={() => conversationsStore.setTagFilter(tag)}
            />
          {/each}
        </div>
      {/if}
    </div>

    <!-- Messages -->
    <div class="flex-1 space-y-4 overflow-y-auto py-6">
      {#each c.messages as message (message.id)}
        <ChatMessage {message} />
      {/each}
    </div>

    <!-- Summary -->
    {#if c.summary}
      <div class="shrink-0 pt-4">
        <SummaryPanel summary={c.summary} tags={c.tags} />
      </div>
    {/if}
  </div>
{:else}
  <div class="flex h-full items-center justify-center">
    <EmptyState
      icon={MessageSquare}
      title="Selecteer een gesprek"
      description="Kies een gesprek uit de lijst om de details te bekijken"
    />
  </div>
{/if}
