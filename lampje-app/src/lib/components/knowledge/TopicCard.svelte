<script lang="ts">
  import type { Topic } from '$lib/types';
  import { formatRelativeTime } from '$lib/utils/formatters';

  let {
    topic,
    index = 0
  }: {
    topic: Topic;
    index?: number;
  } = $props();

  const maxSparkline = $derived(Math.max(...topic.sparkline, 1));
</script>

<div
  class="cursor-pointer rounded-xl bg-surface-2 p-5 transition-all duration-150 hover:shadow-glow-sm"
  style="animation: stagger-in 400ms ease-out both; animation-delay: {index * 60}ms;"
>
  <h3 class="font-display text-base text-on-bg">{topic.name}</h3>

  <div class="mt-3 flex items-center gap-4">
    <span class="font-label text-xs text-on-bg-muted">{topic.conversationCount} gesprekken</span>
    <span class="font-label text-xs text-on-bg-muted">{formatRelativeTime(topic.lastActivity)}</span>
  </div>

  <!-- Sparkline -->
  <div class="mt-3 flex h-6 items-end gap-px">
    {#each topic.sparkline as value}
      <div
        class="w-1 rounded-sm bg-primary/40"
        style="height: {maxSparkline > 0 ? (value / maxSparkline) * 24 : 0}px;"
      ></div>
    {/each}
  </div>
</div>
