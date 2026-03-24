<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';
  import { mockModels } from '$lib/data/mock-settings';
  import { Check } from 'lucide-svelte';
</script>

<div>
  <h2 class="font-display text-lg text-on-bg">AI Model</h2>

  <div class="space-y-3 mt-4">
    {#each mockModels as model}
      {@const isSelected = settingsStore.current.selectedModel === model.id}
      <button
        type="button"
        class="w-full text-left p-4 rounded-xl transition-all duration-150 cursor-pointer {isSelected
          ? 'bg-primary/8 shadow-glow-sm'
          : 'bg-surface-1 hover:bg-surface-bright/50'}"
        onclick={() => settingsStore.updateModel(model.id)}
      >
        <div class="flex justify-between items-start">
          <div class="flex items-center gap-3">
            <div
              class="flex items-center justify-center h-5 w-5 rounded-full shrink-0 transition-colors duration-150 {isSelected
                ? 'bg-primary'
                : 'bg-surface-bright'}"
            >
              {#if isSelected}
                <Check size={12} class="text-on-primary" />
              {/if}
            </div>
            <span class="font-body text-sm font-medium text-on-bg">{model.name}</span>
          </div>
          {#if model.badge}
            <span class="font-label text-xs bg-primary/15 text-primary rounded-full px-2 py-0.5">
              {model.badge}
            </span>
          {/if}
        </div>
        <p class="font-body text-xs text-on-bg-dim mt-1 ml-8">{model.description}</p>
      </button>
    {/each}
  </div>
</div>
