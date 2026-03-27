<script lang="ts">
  import VoiceOrb from '$lib/components/ui/VoiceOrb.svelte';
  import { getDutchGreeting, getDutchDate } from '$lib/utils/greeting';
  import { voiceStore } from '$lib/stores/voice.svelte';

  let { userName = 'daar' }: { userName?: string } = $props();
</script>

<div class="flex justify-between items-start">
  <div>
    <h1 class="font-display text-3xl text-on-bg">{getDutchGreeting()}, {userName}</h1>
    <p class="font-body text-on-bg-dim mt-1">{getDutchDate()}</p>
  </div>
  <VoiceOrb
    size={56}
    state={voiceStore.state}
    onclick={() => voiceStore.simulateCycle()}
  />
</div>
