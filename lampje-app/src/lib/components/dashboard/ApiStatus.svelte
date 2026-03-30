<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';

  const statusConfig = {
    valid: { dotClass: 'bg-primary', label: 'Verbonden', textClass: 'text-primary' },
    invalid: { dotClass: 'bg-error', label: 'Fout', textClass: 'text-error' },
    untested: { dotClass: 'bg-on-bg-muted/40', label: 'Niet getest', textClass: 'text-on-bg-muted' }
  } as const;

  const geminiStatus = $derived(
    settingsStore.current.hasGeminiApiKey ? settingsStore.current.geminiApiKeyStatus : 'untested'
  );
  const geminiConfig = $derived(statusConfig[geminiStatus]);
</script>

<div class="bg-surface-2 rounded-xl p-5">
  <p class="font-label text-xs uppercase tracking-wider text-on-bg-muted">API Status</p>

  <div class="space-y-3 mt-3">
    <div class="flex justify-between items-center">
      <span class="font-body text-sm text-on-bg">Google Gemini</span>
      <div class="flex items-center gap-2">
        {#if settingsStore.current.hasGeminiApiKey}
          <span class="inline-block h-1.5 w-1.5 rounded-full {geminiConfig.dotClass}"></span>
          <span class="font-label text-xs {geminiConfig.textClass}">{geminiConfig.label}</span>
        {:else}
          <span class="inline-block h-1.5 w-1.5 rounded-full bg-on-bg-muted/40"></span>
          <span class="font-label text-xs text-on-bg-muted">Niet ingesteld</span>
        {/if}
      </div>
    </div>

    <div class="flex justify-between items-center">
      <span class="font-body text-sm text-on-bg">PostgreSQL</span>
      <div class="flex items-center gap-2">
        <span class="inline-block h-1.5 w-1.5 rounded-full bg-on-bg-muted/40"></span>
        <span class="font-label text-xs text-on-bg-muted">Niet verbonden</span>
      </div>
    </div>
  </div>
</div>
