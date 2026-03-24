<script lang="ts">
  import VoiceOrb from '$lib/components/ui/VoiceOrb.svelte';
  import { voiceStore } from '$lib/stores/voice.svelte';

  const isActive = $derived(voiceStore.state !== 'idle');

  const statusText = $derived(
    voiceStore.state === 'listening'
      ? 'Ik luister...'
      : voiceStore.state === 'thinking'
        ? 'Even denken...'
        : voiceStore.state === 'speaking'
          ? 'Aan het woord...'
          : 'Klik om te beginnen'
  );

  const statusColor = $derived(
    voiceStore.state === 'listening'
      ? 'text-primary'
      : voiceStore.state === 'thinking'
        ? 'text-secondary'
        : voiceStore.state === 'speaking'
          ? 'text-tertiary'
          : 'text-on-bg-dim'
  );
</script>

<div
  class="bg-surface-2 rounded-xl p-5 transition-all duration-300 {isActive
    ? 'shadow-glow-sm bg-primary/5'
    : ''}"
>
  <p class="font-label text-xs uppercase tracking-wider text-on-bg-muted">Spraakassistent</p>

  <div class="flex items-center gap-4 mt-3">
    <VoiceOrb
      size={48}
      state={voiceStore.state}
      onclick={() => voiceStore.simulateCycle()}
    />
    <span class="font-body text-sm {statusColor}">{statusText}</span>
  </div>
</div>
