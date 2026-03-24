<script lang="ts">
  import VoiceOrb from '$lib/components/ui/VoiceOrb.svelte';
  import { voiceStore } from '$lib/stores/voice.svelte';

  function handleOrbClick() {
    if (voiceStore.state === 'idle') {
      voiceStore.simulateCycle();
    } else {
      voiceStore.stop();
    }
  }
</script>

<style>
  .dot {
    animation: dots-loading 1.4s ease-in-out infinite both;
  }
  .dot:nth-child(1) { animation-delay: 0ms; }
  .dot:nth-child(2) { animation-delay: 160ms; }
  .dot:nth-child(3) { animation-delay: 320ms; }

  @keyframes dots-loading {
    0%, 80%, 100% { opacity: 0.3; }
    40% { opacity: 1; }
  }
</style>

<div class="flex flex-col items-center">
  <VoiceOrb state={voiceStore.state} size={120} onclick={handleOrbClick} />

  <div class="mt-6">
    {#if voiceStore.state === 'idle'}
      <p class="font-body text-lg text-on-bg-dim">Tik om te praten met Lampje</p>
    {:else if voiceStore.state === 'listening'}
      <p class="font-body text-lg text-primary">
        Ik luister<span class="dot">.</span><span class="dot">.</span><span class="dot">.</span>
      </p>
    {:else if voiceStore.state === 'thinking'}
      <p class="font-body text-lg text-secondary">
        Even denken<span class="dot">.</span><span class="dot">.</span><span class="dot">.</span>
      </p>
    {:else if voiceStore.state === 'speaking'}
      <p class="font-body text-lg text-tertiary">Lampje spreekt...</p>
    {/if}
  </div>

  <p class="mt-2 font-label text-xs text-on-bg-muted">Gemini 2.5 Flash &middot; Stem: Puck</p>
</div>
