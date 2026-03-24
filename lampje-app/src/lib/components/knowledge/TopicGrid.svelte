<script lang="ts">
  import type { Topic } from '$lib/types';
  import TopicCard from './TopicCard.svelte';

  let {
    topics
  }: {
    topics: Topic[];
  } = $props();
</script>

<div class="grid grid-cols-1 gap-4 md:grid-cols-2 lg:grid-cols-3">
  {#each topics as topic, i (topic.id)}
    <TopicCard {topic} index={i} />
  {/each}
</div>
