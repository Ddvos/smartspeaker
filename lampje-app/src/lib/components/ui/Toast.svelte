<script lang="ts">
  import { X } from 'lucide-svelte';

  let {
    message,
    type = 'info',
    ondismiss
  }: {
    message: string;
    type?: 'success' | 'error' | 'info';
    ondismiss: () => void;
  } = $props();

  const accentColor = $derived(
    type === 'success' ? 'bg-primary' : type === 'error' ? 'bg-error' : 'bg-tertiary'
  );

  const barColor = $derived(
    type === 'success' ? 'bg-primary' : type === 'error' ? 'bg-error' : 'bg-tertiary'
  );

  let progress = $state(100);

  $effect(() => {
    const startTime = Date.now();
    const totalDuration = 4000;

    const frame = () => {
      const elapsed = Date.now() - startTime;
      progress = Math.max(0, 100 - (elapsed / totalDuration) * 100);

      if (progress <= 0) {
        ondismiss();
        return;
      }

      rafId = requestAnimationFrame(frame);
    };

    let rafId = requestAnimationFrame(frame);

    return () => {
      cancelAnimationFrame(rafId);
    };
  });
</script>

<style>
  .toast-enter {
    animation: toast-slide-in 200ms ease-out both;
  }

  @keyframes toast-slide-in {
    from {
      opacity: 0;
      transform: translateX(100%);
    }
    to {
      opacity: 1;
      transform: translateX(0);
    }
  }
</style>

<div
  class="toast-enter relative overflow-hidden rounded-xl bg-surface-1 px-4 py-3 shadow-glow-sm"
>
  <div class="flex items-center gap-3">
    <!-- Accent dot -->
    <span class="h-2 w-2 shrink-0 rounded-full {accentColor}"></span>

    <!-- Message -->
    <p class="flex-1 font-body text-sm text-on-bg">
      {message}
    </p>

    <!-- Dismiss button -->
    <button
      type="button"
      class="shrink-0 cursor-pointer text-on-bg-muted transition-colors duration-150 hover:text-on-bg-dim"
      onclick={ondismiss}
      aria-label="Sluiten"
    >
      <X size={16} />
    </button>
  </div>

  <!-- Progress bar -->
  <div class="absolute bottom-0 left-0 right-0 h-0.5">
    <div
      class="h-full transition-none {barColor}"
      style="width: {progress}%;"
    ></div>
  </div>
</div>
