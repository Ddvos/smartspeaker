<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';
  import { mockModels, mockVoices } from '$lib/data/mock-settings';

  const currentModel = $derived(
    mockModels.find((m) => m.id === settingsStore.current.selectedModel)
  );
  const currentVoice = $derived(
    mockVoices.find((v) => v.id === settingsStore.current.selectedVoice)
  );
  const personality = $derived(settingsStore.current.personality);
</script>

<div class="bg-surface-2 rounded-xl p-5">
  <p class="font-label text-xs uppercase tracking-wider text-on-bg-muted">Configuratie</p>

  <div class="mt-3">
    <p class="font-label text-xs text-on-bg-muted">AI Model</p>
    <p class="text-on-bg text-sm">{currentModel?.name ?? 'Niet geselecteerd'}</p>
  </div>

  <div class="mt-3">
    <p class="font-label text-xs text-on-bg-muted">Stem</p>
    <p class="text-on-bg text-sm">{currentVoice?.name ?? 'Niet geselecteerd'}</p>
  </div>

  <div class="mt-3">
    <p class="font-label text-xs text-on-bg-muted">Persoonlijkheid</p>
    <div class="h-1.5 rounded-full bg-surface-bright mt-1">
      <div
        class="h-full rounded-full bg-primary transition-all duration-300"
        style="width: {personality}%;"
      ></div>
    </div>
    <div class="flex justify-between mt-1">
      <span class="font-label text-[10px] text-on-bg-muted">Zakelijk</span>
      <span class="font-label text-[10px] text-on-bg-muted">Speels</span>
    </div>
  </div>
</div>
