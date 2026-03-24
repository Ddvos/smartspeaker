<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';

  let value = $state(settingsStore.current.personality);

  function handleInput(e: Event) {
    const target = e.target as HTMLInputElement;
    value = Number(target.value);
    settingsStore.updatePersonality(value);
  }
</script>

<style>
  .personality-slider {
    -webkit-appearance: none;
    appearance: none;
    width: 100%;
    height: 8px;
    border-radius: 9999px;
    background: transparent;
    cursor: pointer;
    position: relative;
    z-index: 2;
  }

  .personality-slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 16px;
    height: 16px;
    border-radius: 9999px;
    background: var(--color-primary);
    cursor: pointer;
    box-shadow: 0 0 12px 0 rgba(129, 236, 255, 0.3);
  }

  .personality-slider::-moz-range-thumb {
    width: 16px;
    height: 16px;
    border-radius: 9999px;
    background: var(--color-primary);
    cursor: pointer;
    border: none;
    box-shadow: 0 0 12px 0 rgba(129, 236, 255, 0.3);
  }
</style>

<div>
  <h2 class="font-display text-lg text-on-bg">Persoonlijkheid</h2>

  <div class="flex justify-between mt-4">
    <span class="font-label text-xs text-on-bg-muted">Zakelijk</span>
    <span class="font-label text-xs text-on-bg-muted">Speels</span>
  </div>

  <div class="relative mt-2">
    <div class="absolute top-1/2 -translate-y-1/2 left-0 right-0 h-2 rounded-full bg-surface-bright">
      <div
        class="h-full rounded-full bg-primary transition-all duration-150"
        style="width: {value}%;"
      ></div>
    </div>
    <input
      type="range"
      min="0"
      max="100"
      step="1"
      {value}
      oninput={handleInput}
      class="personality-slider"
    />
  </div>

  <p class="font-label text-sm text-primary text-center mt-2">{value}%</p>
</div>
