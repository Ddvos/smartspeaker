<script lang="ts">
  import { goto } from '$app/navigation';
  import { mockConversations } from '$lib/data/mock-conversations';
  import { formatRelativeTime } from '$lib/utils/formatters';
  import TagChip from '$lib/components/ui/TagChip.svelte';
  import type { Mood } from '$lib/types';

  const moodColors: Record<Mood, string> = {
    focused: '#81ecff',
    happy: '#ff7520',
    curious: '#a68cff',
    playful: '#ff716c',
    neutral: '#6b7280'
  };

  const sorted = $derived(
    [...mockConversations].sort((a, b) => b.updatedAt.getTime() - a.updatedAt.getTime()).slice(0, 5)
  );
</script>

<div class="bg-surface-2 rounded-xl p-6">
  <div class="flex justify-between items-center">
    <h2 class="font-display text-lg text-on-bg">Recente gesprekken</h2>
    <a href="/conversations" class="text-primary text-sm font-body hover:text-primary-dim transition-colors duration-150">
      Alles bekijken &rarr;
    </a>
  </div>

  <div class="space-y-1 mt-4">
    {#each sorted as conversation}
      <button
        type="button"
        class="flex items-center gap-3 px-3 py-3 rounded-xl hover:bg-surface-bright/50 transition-colors cursor-pointer group w-full text-left"
        onclick={() => goto('/conversations')}
      >
        <span
          class="shrink-0 rounded-full"
          style="width: 8px; height: 8px; background-color: {moodColors[conversation.mood]};"
        ></span>

        <div class="flex-1 min-w-0">
          <p class="font-body text-sm font-medium text-on-bg truncate">{conversation.title}</p>
          <div class="flex gap-1.5 mt-1">
            {#each conversation.tags as tag}
              <TagChip label={tag} />
            {/each}
          </div>
        </div>

        <div class="text-right shrink-0">
          <p class="font-label text-xs text-on-bg-muted">{formatRelativeTime(conversation.updatedAt)}</p>
          <p class="font-label text-xs text-on-bg-muted">{conversation.messages.length} berichten</p>
        </div>
      </button>
    {/each}
  </div>
</div>
