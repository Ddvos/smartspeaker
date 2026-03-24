<script lang="ts">
  import SearchBar from '$lib/components/ui/SearchBar.svelte';
  import TopicGrid from '$lib/components/knowledge/TopicGrid.svelte';
  import TimelineView from '$lib/components/knowledge/TimelineView.svelte';
  import { mockTopicCards, mockTimeline } from '$lib/data/mock-knowledge';

  let viewMode = $state<'topics' | 'timeline'>('topics');
</script>

<div class="mx-auto max-w-[960px] space-y-6">
  <h1 class="font-display text-3xl text-on-bg">Kennisbank</h1>

  <!-- Large search bar -->
  <SearchBar
    placeholder="Vraag me iets... bijv. 'Wat weet ik over LiFePO4 batterijen?'"
    size="large"
    showShortcut={false}
  />

  <!-- View toggle -->
  <div class="flex items-center gap-2">
    <span class="font-label text-xs text-on-bg-muted">Weergave:</span>
    <button
      type="button"
      class="cursor-pointer rounded-lg px-3 py-1.5 font-label text-xs transition-colors {viewMode === 'topics' ? 'bg-primary/10 text-primary' : 'bg-surface-2 text-on-bg-muted hover:text-on-bg-dim'}"
      onclick={() => (viewMode = 'topics')}
    >
      Topics
    </button>
    <button
      type="button"
      class="cursor-pointer rounded-lg px-3 py-1.5 font-label text-xs transition-colors {viewMode === 'timeline' ? 'bg-primary/10 text-primary' : 'bg-surface-2 text-on-bg-muted hover:text-on-bg-dim'}"
      onclick={() => (viewMode = 'timeline')}
    >
      Tijdlijn
    </button>
  </div>

  {#if viewMode === 'topics'}
    <TopicGrid topics={mockTopicCards} />
  {:else}
    <TimelineView entries={mockTimeline} />
  {/if}
</div>
