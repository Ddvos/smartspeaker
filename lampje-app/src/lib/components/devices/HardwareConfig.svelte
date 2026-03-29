<script lang="ts">
  import type { DeviceConfig } from '$lib/types';
  import { deviceStore } from '$lib/stores/devices.svelte';

  let { config, deviceId }: { config: DeviceConfig; deviceId: string } = $props();

  let ledColors = $state({ ...config.ledColors });
  let micSensitivity = $state(config.micSensitivity);
  let speakerVolume = $state(config.speakerVolume);
  let displayBrightness = $state(config.displayBrightness);

  $effect(() => {
    ledColors = { ...config.ledColors };
    micSensitivity = config.micSensitivity;
    speakerVolume = config.speakerVolume;
    displayBrightness = config.displayBrightness;
  });

  const ledLabels: Record<string, string> = {
    idle: 'Inactief',
    listening: 'Luistert',
    thinking: 'Denkt',
    speaking: 'Spreekt',
    error: 'Fout'
  };

  const micLabel = $derived(
    micSensitivity < 33 ? 'Laag' : micSensitivity < 66 ? 'Gemiddeld' : 'Hoog'
  );

  const volumeLabel = $derived(
    speakerVolume < 33 ? 'Laag' : speakerVolume < 66 ? 'Gemiddeld' : 'Hoog'
  );

  const brightnessLabel = $derived(
    displayBrightness < 33 ? 'Laag' : displayBrightness < 66 ? 'Gemiddeld' : 'Hoog'
  );

  let saveTimeout: ReturnType<typeof setTimeout> | null = null;

  function debounceSave() {
    if (saveTimeout) clearTimeout(saveTimeout);
    saveTimeout = setTimeout(() => {
      deviceStore.updateConfig(deviceId, {
        ledColors,
        micSensitivity,
        speakerVolume,
        displayBrightness
      });
    }, 500);
  }
</script>

<style>
  input[type='range'] {
    -webkit-appearance: none;
    appearance: none;
    width: 100%;
    height: 6px;
    border-radius: 9999px;
    background: var(--color-surface-bright);
    outline: none;
  }

  input[type='range']::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 18px;
    height: 18px;
    border-radius: 50%;
    background: var(--color-primary);
    cursor: pointer;
  }

  input[type='range']::-moz-range-thumb {
    width: 18px;
    height: 18px;
    border-radius: 50%;
    background: var(--color-primary);
    cursor: pointer;
    border: none;
  }

  input[type='color'] {
    -webkit-appearance: none;
    appearance: none;
    border: none;
    padding: 0;
    background: transparent;
  }

  input[type='color']::-webkit-color-swatch-wrapper {
    padding: 0;
  }

  input[type='color']::-webkit-color-swatch {
    border: none;
    border-radius: 0.5rem;
  }
</style>

<div class="rounded-xl bg-surface-2 p-6">
  <h2 class="font-display text-lg text-on-bg">Hardware configuratie</h2>

  <div class="mt-4 space-y-6">
    <!-- LED Color Mapping -->
    <div>
      <h3 class="mb-3 font-label text-xs uppercase tracking-wider text-on-bg-muted">LED Kleur Mapping</h3>
      <div class="space-y-3">
        {#each Object.entries(ledLabels) as [key, label]}
          <div class="flex items-center justify-between">
            <span class="font-body text-sm text-on-bg">{label}</span>
            <input
              type="color"
              class="h-8 w-8 cursor-pointer rounded-lg"
              bind:value={ledColors[key]}
              onchange={debounceSave}
            />
          </div>
        {/each}
      </div>
    </div>

    <!-- Microphone Sensitivity -->
    <div>
      <div class="mb-2 flex items-center justify-between">
        <span class="font-body text-sm text-on-bg">Microfoon gevoeligheid</span>
        <span class="font-label text-xs text-on-bg-muted">{micLabel}</span>
      </div>
      <input
        type="range"
        min="0"
        max="100"
        bind:value={micSensitivity}
        oninput={debounceSave}
      />
    </div>

    <!-- Speaker Volume -->
    <div>
      <div class="mb-2 flex items-center justify-between">
        <span class="font-body text-sm text-on-bg">Speaker volume</span>
        <span class="font-label text-xs text-on-bg-muted">{volumeLabel}</span>
      </div>
      <input
        type="range"
        min="0"
        max="100"
        bind:value={speakerVolume}
        oninput={debounceSave}
      />
    </div>

    <!-- Display Brightness -->
    <div>
      <div class="mb-2 flex items-center justify-between">
        <span class="font-body text-sm text-on-bg">Scherm helderheid</span>
        <span class="font-label text-xs text-on-bg-muted">{brightnessLabel}</span>
      </div>
      <input
        type="range"
        min="0"
        max="100"
        bind:value={displayBrightness}
        oninput={debounceSave}
      />
    </div>
  </div>
</div>
