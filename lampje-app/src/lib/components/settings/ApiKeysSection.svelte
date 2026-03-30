<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';
  import { toastStore } from '$lib/stores/toast.svelte';

  let keyInput = $state('');
  let testing = $state(false);

  async function handleSave() {
    if (!keyInput.trim()) return;
    await settingsStore.saveApiKey(keyInput);
    keyInput = '';
    toastStore.success('API-sleutel opgeslagen');
  }

  async function handleTest() {
    testing = true;
    const success = await settingsStore.testApiKey();
    if (success) {
      toastStore.success('API-sleutel succesvol verbonden');
    } else {
      toastStore.error('Kon niet verbinden met API');
    }
    testing = false;
  }

  async function handleClear() {
    await settingsStore.saveApiKey('');
    keyInput = '';
    toastStore.success('API-sleutel verwijderd');
  }
</script>

<div class="space-y-4">
  <div>
    <label class="font-label text-sm text-on-bg font-medium" for="api-key-gemini">Google Gemini</label>

    {#if settingsStore.current.hasGeminiApiKey}
      <div class="flex items-center gap-3 mt-2">
        <div class="bg-surface-0 rounded-xl px-4 py-2.5 text-on-bg-dim font-body text-sm flex-1">
          {settingsStore.current.geminiApiKeyMasked}
        </div>
        <button
          type="button"
          class="bg-surface-bright text-on-bg-dim rounded-xl px-4 py-2.5 font-body text-sm hover:bg-surface-2 cursor-pointer transition-colors duration-150 disabled:opacity-50"
          disabled={testing}
          onclick={handleTest}
        >
          {testing ? 'Testen...' : 'Test'}
        </button>
        <button
          type="button"
          class="text-on-bg-muted hover:text-error rounded-xl px-3 py-2.5 font-body text-sm cursor-pointer transition-colors duration-150"
          onclick={handleClear}
        >
          Wissen
        </button>
      </div>
    {:else}
      <div class="flex gap-3 mt-2">
        <input
          id="api-key-gemini"
          type="password"
          class="bg-surface-0 rounded-xl px-4 py-2.5 text-on-bg font-body text-sm flex-1 focus:ring-2 focus:ring-primary/20 outline-none"
          placeholder="Plak je API-sleutel..."
          bind:value={keyInput}
          onkeydown={(e: KeyboardEvent) => { if (e.key === 'Enter') handleSave(); }}
        />
        <button
          type="button"
          class="bg-primary text-on-primary rounded-xl px-4 py-2.5 font-body text-sm hover:bg-primary/90 cursor-pointer transition-colors duration-150 disabled:opacity-50"
          disabled={!keyInput.trim() || settingsStore.saving}
          onclick={handleSave}
        >
          Opslaan
        </button>
      </div>
    {/if}

    {#if settingsStore.current.geminiApiKeyStatus === 'valid'}
      <span class="inline-block mt-2 text-primary bg-primary/10 rounded-full px-3 py-1 font-label text-xs">
        Verbonden
      </span>
    {:else if settingsStore.current.geminiApiKeyStatus === 'invalid'}
      <span class="inline-block mt-2 text-error bg-error/10 rounded-full px-3 py-1 font-label text-xs">
        Ongeldige sleutel
      </span>
    {:else if settingsStore.current.hasGeminiApiKey}
      <span class="inline-block mt-2 text-on-bg-muted bg-surface-bright rounded-full px-3 py-1 font-label text-xs">
        Niet getest
      </span>
    {/if}
  </div>
</div>
