<script lang="ts">
  import { mockActivity } from '$lib/data/mock-dashboard';

  let activeTab = $state<'minutes' | 'conversations'>('minutes');

  const maxValue = $derived(
    Math.max(...mockActivity.map((d) => (activeTab === 'minutes' ? d.minutes : d.conversations)))
  );
</script>

<div class="bg-surface-2 rounded-xl p-6">
  <div class="flex justify-between items-center">
    <h2 class="font-display text-lg text-on-bg">Activiteit</h2>
    <div class="flex gap-1">
      <button
        type="button"
        class="rounded-lg px-3 py-1 text-sm font-body cursor-pointer transition-colors duration-150 {activeTab === 'minutes'
          ? 'text-primary bg-primary/10'
          : 'text-on-bg-muted hover:text-on-bg-dim'}"
        onclick={() => (activeTab = 'minutes')}
      >
        Minuten
      </button>
      <button
        type="button"
        class="rounded-lg px-3 py-1 text-sm font-body cursor-pointer transition-colors duration-150 {activeTab === 'conversations'
          ? 'text-primary bg-primary/10'
          : 'text-on-bg-muted hover:text-on-bg-dim'}"
        onclick={() => (activeTab = 'conversations')}
      >
        Gesprekken
      </button>
    </div>
  </div>

  <div class="flex items-end gap-3 h-[200px] mt-6">
    {#each mockActivity as day}
      {@const value = activeTab === 'minutes' ? day.minutes : day.conversations}
      {@const percentage = maxValue > 0 ? (value / maxValue) * 100 : 0}
      <div class="flex-1 flex flex-col items-center gap-2 h-full justify-end">
        <span class="font-label text-xs text-on-bg-dim">{value}</span>
        <div
          class="w-full rounded-t-lg bg-gradient-to-t from-primary-dim to-primary transition-all duration-500 ease-out"
          style="height: {percentage}%;"
        ></div>
        <span class="font-label text-xs text-on-bg-muted">{day.label}</span>
      </div>
    {/each}
  </div>
</div>
