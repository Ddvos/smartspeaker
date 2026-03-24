<script lang="ts">
  import { settingsStore } from '$lib/stores/settings.svelte';

  const statusConfig = {
    connected: { dotClass: 'bg-primary', label: 'Verbonden', textClass: 'text-primary' },
    error: { dotClass: 'bg-error', label: 'Fout', textClass: 'text-error' },
    untested: { dotClass: 'bg-on-bg-muted/40', label: 'Niet getest', textClass: 'text-on-bg-muted' }
  } as const;
</script>

<div class="bg-surface-2 rounded-xl p-5">
  <p class="font-label text-xs uppercase tracking-wider text-on-bg-muted">API Status</p>

  <div class="space-y-3 mt-3">
    {#each settingsStore.current.apiKeys as apiKey}
      {@const config = statusConfig[apiKey.status]}
      <div class="flex justify-between items-center">
        <span class="font-body text-sm text-on-bg">{apiKey.label}</span>
        <div class="flex items-center gap-2">
          <span class="inline-block h-1.5 w-1.5 rounded-full {config.dotClass}"></span>
          <span class="font-label text-xs {config.textClass}">{config.label}</span>
        </div>
      </div>
    {/each}

    <div class="flex justify-between items-center">
      <span class="font-body text-sm text-on-bg">PostgreSQL</span>
      <div class="flex items-center gap-2">
        <span class="inline-block h-1.5 w-1.5 rounded-full bg-on-bg-muted/40"></span>
        <span class="font-label text-xs text-on-bg-muted">Niet verbonden</span>
      </div>
    </div>
  </div>
</div>
