<script lang="ts">
  import { Mic, Loader, Volume2 } from 'lucide-svelte';
  import posthog from 'posthog-js';
  import { browser } from '$app/environment';
  import { experimentStore } from '$lib/stores/experiment.svelte';

  let {
    state = 'idle',
    size = 64,
    onclick
  }: {
    state?: 'idle' | 'listening' | 'thinking' | 'speaking';
    size?: number;
    onclick?: () => void;
  } = $props();

  const hoverVariant = $derived(experimentStore.micHoverVariant);

  const innerSize = $derived(size - 6);

  const iconSize = $derived(Math.round(size * 0.35));

  const iconColor = $derived(
    state === 'idle'
      ? 'text-on-bg-muted'
      : state === 'listening'
        ? 'text-primary'
        : state === 'thinking'
          ? 'text-secondary'
          : 'text-tertiary'
  );

  const outerAnimation = $derived(
    state === 'idle'
      ? 'animation: orb-breathe 4s ease-in-out infinite;'
      : state === 'listening'
        ? 'animation: orb-listening 1.5s ease-in-out infinite;'
        : state === 'speaking'
          ? 'animation: orb-speaking 0.8s ease-in-out infinite;'
          : ''
  );

  const shadowClass = $derived(
    state === 'listening'
      ? 'shadow-glow-md'
      : state === 'thinking'
        ? 'shadow-glow-secondary'
        : state === 'speaking'
          ? 'shadow-glow-tertiary'
          : ''
  );

  const innerRounding = $derived(state === 'thinking' ? 'rounded-2xl' : 'rounded-full');

  const hoverGlow = $derived(
    state !== 'idle'
      ? 'transparent'
      : hoverVariant === 'test'
        ? 'rgba(166, 140, 255, 0.7)'
        : 'rgba(255, 117, 32, 0.7)'
  );

  function handleClick() {
    if (browser) {
      posthog.capture('mic_button_clicked', {
        variant: hoverVariant ?? 'unknown',
        orb_location: size >= 100 ? 'voicelab' : size >= 56 ? 'header' : 'sidebar'
      });
    }
    onclick?.();
  }
</script>

<style>
  @keyframes orb-speaking {
    0%,
    100% {
      transform: scale(1);
      box-shadow: 0 0 24px rgba(166, 140, 255, 0.2);
    }
    50% {
      transform: scale(1.04);
      box-shadow: 0 0 48px rgba(166, 140, 255, 0.4);
    }
  }

  .orb-btn:hover {
    box-shadow: 0 0 32px 8px var(--hover-glow);
  }

  .spin {
    animation: spin 1.2s linear infinite;
  }

  @keyframes spin {
    from {
      transform: rotate(0deg);
    }
    to {
      transform: rotate(360deg);
    }
  }
</style>

<button
  type="button"
  class="orb-btn relative flex cursor-pointer items-center justify-center rounded-full bg-gradient-to-br from-primary to-primary-dim transition-all duration-300 ease-[cubic-bezier(0.23,1,0.32,1)] focus-visible:outline-none focus-visible:ring-2 focus-visible:ring-primary/40 focus-visible:ring-offset-2 focus-visible:ring-offset-background {shadowClass}"
  style="width: {size}px; height: {size}px; --hover-glow: {hoverGlow}; {outerAnimation}"
  onclick={handleClick}
  aria-label={state === 'idle'
    ? 'Microfoon activeren'
    : state === 'listening'
      ? 'Luistert...'
      : state === 'thinking'
        ? 'Nadenken...'
        : 'Spreekt...'}
>
  <div
    class="flex items-center justify-center bg-surface-1 transition-all duration-300 ease-[cubic-bezier(0.23,1,0.32,1)] {innerRounding}"
    style="width: {innerSize}px; height: {innerSize}px;"
  >
    <div class={iconColor} class:spin={state === 'thinking'}>
      {#if state === 'thinking'}
        <Loader size={iconSize} strokeWidth={2} />
      {:else if state === 'speaking'}
        <Volume2 size={iconSize} strokeWidth={2} />
      {:else}
        <Mic size={iconSize} strokeWidth={2} />
      {/if}
    </div>
  </div>
</button>
