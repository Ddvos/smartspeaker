<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';
  import { toastStore } from '$lib/stores/toast.svelte';

  let testing = $state<string | null>(null);

  async function handleTest(provider: string) {
    testing = provider;
    const success = await settingsStore.testApiKey(provider);
    if (success) {
      toastStore.success('API-sleutel succesvol verbonden');
    } else {
      toastStore.error('Kon niet verbinden met API');
    }
    testing = null;
  }
</script>

<div class="space-y-5">
  {#each settingsStore.current.apiKeys as apiKey}
    <div>
      <label class="font-label text-sm text-on-bg font-medium" for="api-key-{apiKey.provider}">{apiKey.label}</label>
      <div class="flex gap-3 mt-2">
        <input
          id="api-key-{apiKey.provider}"
          type="password"
          class="bg-surface-0 rounded-xl px-4 py-2.5 text-on-bg font-body text-sm flex-1 focus:ring-2 focus:ring-primary/20 outline-none"
          placeholder="Plak je API-sleutel..."
          value={apiKey.key}
          oninput={(e: Event) => {
            const target = e.target as HTMLInputElement;
            settingsStore.updateApiKey(apiKey.provider, target.value);
          }}
        />
        <button
          type="button"
          class="bg-surface-bright text-on-bg-dim rounded-xl px-4 py-2.5 font-body text-sm hover:bg-surface-2 cursor-pointer transition-colors duration-150 disabled:opacity-50"
          disabled={testing === apiKey.provider}
          onclick={() => handleTest(apiKey.provider)}
        >
          {testing === apiKey.provider ? 'Testen...' : 'Test'}
        </button>
      </div>
      {#if apiKey.status === 'connected'}
        <span class="inline-block mt-2 text-primary bg-primary/10 rounded-full px-3 py-1 font-label text-xs">
          Verbonden
        </span>
      {:else if apiKey.status === 'error'}
        <span class="inline-block mt-2 text-error bg-error/10 rounded-full px-3 py-1 font-label text-xs">
          Fout
        </span>
      {/if}
    </div>
  {/each}
</div>
