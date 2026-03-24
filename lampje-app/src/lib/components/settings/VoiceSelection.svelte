<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';
  import { mockVoices } from '$lib/data/mock-settings';
</script>

<div>
  <h2 class="font-display text-lg text-on-bg">Stem selectie</h2>

  <div class="grid grid-cols-2 gap-3 mt-4">
    {#each mockVoices as voice}
      {@const isSelected = settingsStore.current.selectedVoice === voice.id}
      <button
        type="button"
        class="p-4 rounded-xl transition-all duration-150 text-left cursor-pointer {isSelected
          ? 'bg-tertiary/10 shadow-glow-tertiary'
          : 'bg-surface-1 hover:bg-surface-bright/50'}"
        onclick={() => settingsStore.updateVoice(voice.id)}
      >
        <p class="font-body text-sm font-medium text-on-bg">{voice.name}</p>
        <p class="font-body text-xs text-on-bg-dim mt-1">{voice.style}</p>
      </button>
    {/each}
  </div>
</div>
