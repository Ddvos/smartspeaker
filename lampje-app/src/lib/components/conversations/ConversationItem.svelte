<script lang="ts">
  import type { Conversation } from '$lib/types';
  import TagChip from '$lib/components/ui/TagChip.svelte';
  import { formatDate, formatDuration } from '$lib/utils/formatters';

  let {
    conversation,
    isSelected = false,
    onclick
  }: {
    conversation: Conversation;
    isSelected?: boolean;
    onclick: () => void;
  } = $props();

  const moodColors: Record<string, string> = {
    focused: '#81ecff',
    happy: '#ff7520',
    curious: '#a68cff',
    playful: '#ff716c',
    neutral: '#6b7280'
  };

  const moodColor = $derived(moodColors[conversation.mood] ?? '#6b7280');
  const durationMinutes = $derived(Math.floor(conversation.duration / 60));
</script>

<button
  type="button"
  class="relative flex w-full cursor-pointer items-start gap-3 rounded-xl px-4 py-3.5 text-left transition-all duration-150 {isSelected ? 'bg-primary/8' : 'hover:bg-surface-1/50'}"
  {onclick}
>
  {#if isSelected}
    <div class="absolute left-0 top-1/2 h-full w-0.5 -translate-y-1/2 rounded-full bg-primary"></div>
  {/if}

  <!-- Mood dot -->
  <span
    class="mt-2 h-2 w-2 shrink-0 rounded-full"
    style="background-color: {moodColor};"
  ></span>

  <!-- Content -->
  <div class="min-w-0 flex-1">
    <p class="truncate font-body text-sm font-medium text-on-bg">{conversation.title}</p>
    {#if conversation.summary}
      <p class="mt-0.5 line-clamp-1 font-body text-xs text-on-bg-muted">{conversation.summary}</p>
    {/if}
    {#if conversation.tags.length > 0}
      <div class="mt-1.5 flex flex-wrap gap-1">
        {#each conversation.tags as tag}
          <TagChip label={tag} />
        {/each}
      </div>
    {/if}
  </div>

  <!-- Right side -->
  <div class="shrink-0 text-right">
    <p class="font-label text-[11px] text-on-bg-muted">
      {formatDate(conversation.createdAt)} &middot; {durationMinutes} min
    </p>
    <p class="mt-0.5 font-label text-[11px] text-on-bg-muted">
      {conversation.messages.length} berichten
    </p>
  </div>
</button>
