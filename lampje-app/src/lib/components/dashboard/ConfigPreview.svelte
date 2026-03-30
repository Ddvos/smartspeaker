<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';
  import { models, voices } from '$lib/data/settings-constants';

  const currentModel = $derived(
    models.find((m) => m.id === settingsStore.current.defaultModel)
  );
  const currentVoice = $derived(
    voices.find((v) => v.id === settingsStore.current.defaultVoice)
  );
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
    <p class="font-label text-xs text-on-bg-muted">API Sleutel</p>
    <p class="text-on-bg text-sm">
      {#if settingsStore.current.geminiApiKeyStatus === 'valid'}
        Verbonden
      {:else if settingsStore.current.hasGeminiApiKey}
        Niet getest
      {:else}
        Niet ingesteld
      {/if}
    </p>
  </div>
</div>
