<script lang="ts">
  let ledColors = $state({
    idle: '#6b7280',
    listening: '#81ecff',
    thinking: '#ff7520',
    speaking: '#a68cff',
    error: '#ff716c'
  });

  const ledLabels: Record<string, string> = {
    idle: 'Inactief',
    listening: 'Luistert',
    thinking: 'Denkt',
    speaking: 'Spreekt',
    error: 'Fout'
  };

  let micSensitivity = $state(50);
  let speakerVolume = $state(75);
  let cameraEnabled = $state(false);
  let cameraResolution = $state('480p');
  let cliffSensors = $state(true);

  const micLabel = $derived(
    micSensitivity < 33 ? 'Laag' : micSensitivity < 66 ? 'Gemiddeld' : 'Hoog'
  );

  const volumeLabel = $derived(
    speakerVolume < 33 ? 'Laag' : speakerVolume < 66 ? 'Gemiddeld' : 'Hoog'
  );

  const resolutionOptions = ['240p', '480p', '720p'] as const;
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
              bind:value={ledColors[key as keyof typeof ledColors]}
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
      />
    </div>

    <!-- Camera -->
    <div>
      <div class="flex items-center justify-between">
        <span class="font-body text-sm text-on-bg">Camera</span>
        <button
          type="button"
          class="relative h-6 w-11 cursor-pointer rounded-full transition-colors duration-200 {cameraEnabled ? 'bg-primary' : 'bg-surface-bright'}"
          onclick={() => (cameraEnabled = !cameraEnabled)}
          role="switch"
          aria-checked={cameraEnabled}
          aria-label="Camera aan/uit"
        >
          <span
            class="absolute left-0.5 top-0.5 h-5 w-5 rounded-full bg-on-bg transition-transform duration-200 {cameraEnabled ? 'translate-x-5' : 'translate-x-0'}"
          ></span>
        </button>
      </div>

      {#if cameraEnabled}
        <div class="mt-3 flex items-center gap-2">
          <span class="font-label text-xs text-on-bg-muted">Resolutie:</span>
          {#each resolutionOptions as res}
            <button
              type="button"
              class="cursor-pointer rounded-lg px-3 py-1.5 font-label text-xs transition-colors {cameraResolution === res ? 'bg-primary/10 text-primary' : 'bg-surface-bright text-on-bg-muted hover:text-on-bg-dim'}"
              onclick={() => (cameraResolution = res)}
            >
              {res}
            </button>
          {/each}
        </div>
      {/if}
    </div>

    <!-- Cliff Sensors -->
    <div class="flex items-center justify-between">
      <span class="font-body text-sm text-on-bg">Cliff sensoren</span>
      <button
        type="button"
        class="relative h-6 w-11 cursor-pointer rounded-full transition-colors duration-200 {cliffSensors ? 'bg-primary' : 'bg-surface-bright'}"
        onclick={() => (cliffSensors = !cliffSensors)}
        role="switch"
        aria-checked={cliffSensors}
        aria-label="Cliff sensoren aan/uit"
      >
        <span
          class="absolute left-0.5 top-0.5 h-5 w-5 rounded-full bg-on-bg transition-transform duration-200 {cliffSensors ? 'translate-x-5' : 'translate-x-0'}"
        ></span>
      </button>
    </div>
  </div>
</div>
