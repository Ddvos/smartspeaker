<script lang="ts">
  import { mockTopics } from '$lib/data/mock-dashboard';

  const maxCount = $derived(Math.max(...mockTopics.map((t) => t.count)));
</script>

<div class="bg-surface-2 rounded-xl p-6">
  <h2 class="font-display text-lg text-on-bg mb-4">Meest besproken</h2>

  <div class="space-y-4">
    {#each mockTopics as topic}
      {@const percentage = maxCount > 0 ? (topic.count / maxCount) * 100 : 0}
      <div>
        <div class="flex justify-between items-center">
          <span class="font-body text-sm text-on-bg">{topic.name}</span>
          <span class="font-label text-xs text-on-bg-muted tabular-nums">{topic.count}</span>
        </div>
        <div class="h-1 rounded-full bg-surface-bright mt-2">
          <div
            class="h-full rounded-full transition-all duration-700 ease-out"
            style="width: {percentage}%; background-color: {topic.color};"
          ></div>
        </div>
      </div>
    {/each}
  </div>
</div>
