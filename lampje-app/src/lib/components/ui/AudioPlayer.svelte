<script lang="ts">
  import { Play, Pause } from 'lucide-svelte';

  let {
    duration = 120
  }: {
    duration?: number;
  } = $props();

  let playing = $state(false);
  let currentTime = $state(0);
  let speed = $state(1);
  let intervalId: ReturnType<typeof setInterval> | null = null;

  const barHeights = $state(
    Array.from({ length: 20 }, () => Math.floor(Math.random() * 16) + 4)
  );

  const speedOptions = [1, 1.5, 2] as const;

  function formatTime(seconds: number): string {
    const mins = Math.floor(seconds / 60);
    const secs = Math.floor(seconds % 60);
    return `${mins}:${secs.toString().padStart(2, '0')}`;
  }

  function togglePlay() {
    playing = !playing;
  }

  function cycleSpeed() {
    const currentIndex = speedOptions.indexOf(speed as (typeof speedOptions)[number]);
    const nextIndex = (currentIndex + 1) % speedOptions.length;
    speed = speedOptions[nextIndex];
  }

  const progress = $derived(duration > 0 ? currentTime / duration : 0);
  const activeBarIndex = $derived(Math.floor(progress * barHeights.length));

  $effect(() => {
    if (playing) {
      intervalId = setInterval(() => {
        currentTime += 0.1 * speed;
        if (currentTime >= duration) {
          currentTime = duration;
          playing = false;
        }
      }, 100);
    } else {
      if (intervalId) {
        clearInterval(intervalId);
        intervalId = null;
      }
    }

    return () => {
      if (intervalId) {
        clearInterval(intervalId);
        intervalId = null;
      }
    };
  });
</script>

<div class="flex items-center gap-3 rounded-lg bg-surface-1 p-3">
  <!-- Play/Pause button -->
  <button
    type="button"
    class="flex h-8 w-8 shrink-0 cursor-pointer items-center justify-center rounded-full bg-surface-2 transition-colors duration-150 hover:bg-surface-bright"
    onclick={togglePlay}
    aria-label={playing ? 'Pauzeren' : 'Afspelen'}
  >
    {#if playing}
      <Pause size={16} class="text-on-bg" />
    {:else}
      <Play size={16} class="text-on-bg ml-0.5" />
    {/if}
  </button>

  <!-- Waveform -->
  <div class="flex flex-1 items-center gap-[2px]" style="height: 24px;">
    {#each barHeights as height, i}
      <div
        class="w-[2px] shrink-0 rounded-full transition-colors duration-100 {i < activeBarIndex
          ? 'bg-primary'
          : 'bg-on-bg-muted/30'}"
        style="height: {height}px;"
      ></div>
    {/each}
  </div>

  <!-- Time display -->
  <span class="shrink-0 font-label text-xs text-on-bg-dim">
    {formatTime(currentTime)} / {formatTime(duration)}
  </span>

  <!-- Speed button -->
  <button
    type="button"
    class="shrink-0 cursor-pointer rounded-lg bg-surface-2 px-2 py-1 font-label text-xs text-on-bg-dim transition-colors duration-150 hover:bg-surface-bright"
    onclick={cycleSpeed}
    aria-label="Afspeelsnelheid wijzigen"
  >
    {speed}x
  </button>
</div>
