<script lang="ts">
  import { Download, Trash2, Cookie } from 'lucide-svelte';
  import { toastStore } from '$lib/stores/toast.svelte';
  import { consentStore } from '$lib/stores/consent.svelte';
  import ConfirmDialog from '$lib/components/ui/ConfirmDialog.svelte';

  let deleteConversationsOpen = $state(false);
  let deleteEmbeddingsOpen = $state(false);

  function handleExport() {
    toastStore.info('Export gestart...');
  }

  function handleDeleteConversations() {
    deleteConversationsOpen = false;
    toastStore.success('Alle gesprekken zijn verwijderd');
  }

  function handleDeleteEmbeddings() {
    deleteEmbeddingsOpen = false;
    toastStore.success('Alle embeddings zijn verwijderd');
  }
</script>

<div>
  <h2 class="font-display text-lg text-on-bg">Data & Privacy</h2>

  <div class="space-y-3 mt-4">
    <div class="bg-surface-1 rounded-xl px-4 py-3 flex items-center justify-between gap-3">
      <div class="flex items-center gap-3">
        <Cookie size={18} class="text-on-bg-dim shrink-0" />
        <div>
          <span class="font-body text-sm text-on-bg">Analytics cookies</span>
          <p class="font-body text-xs text-on-bg-dim mt-0.5">
            {#if consentStore.status === 'granted'}
              Actief — we verzamelen analytics data
            {:else}
              Inactief — alleen anonieme data
            {/if}
          </p>
        </div>
      </div>
      {#if consentStore.status === 'granted'}
        <button
          type="button"
          class="cursor-pointer rounded-lg bg-surface-2 px-3 py-1.5 font-body text-xs text-on-bg-dim transition-colors duration-150 hover:bg-surface-bright"
          onclick={() => {
            consentStore.withdraw();
            toastStore.success('Analytics cookies ingetrokken');
          }}
        >
          Intrekken
        </button>
      {:else}
        <button
          type="button"
          class="cursor-pointer rounded-lg bg-primary/15 px-3 py-1.5 font-body text-xs text-primary transition-colors duration-150 hover:bg-primary/25"
          onclick={() => {
            consentStore.accept();
            toastStore.success('Analytics cookies geaccepteerd');
          }}
        >
          Accepteren
        </button>
      {/if}
    </div>

    <button
      type="button"
      class="bg-surface-1 hover:bg-surface-bright rounded-xl px-4 py-3 w-full text-left flex items-center gap-3 cursor-pointer transition-colors duration-150"
      onclick={handleExport}
    >
      <Download size={18} class="text-on-bg-dim shrink-0" />
      <span class="font-body text-sm text-on-bg">Exporteer alle data</span>
    </button>

    <button
      type="button"
      class="bg-error/10 hover:bg-error/20 text-error rounded-xl px-4 py-3 w-full text-left flex items-center gap-3 cursor-pointer transition-colors duration-150"
      onclick={() => (deleteConversationsOpen = true)}
    >
      <Trash2 size={18} class="shrink-0" />
      <span class="font-body text-sm">Verwijder alle gesprekken</span>
    </button>

    <button
      type="button"
      class="bg-error/10 hover:bg-error/20 text-error rounded-xl px-4 py-3 w-full text-left flex items-center gap-3 cursor-pointer transition-colors duration-150"
      onclick={() => (deleteEmbeddingsOpen = true)}
    >
      <Trash2 size={18} class="shrink-0" />
      <span class="font-body text-sm">Verwijder alle embeddings</span>
    </button>
  </div>
</div>

<ConfirmDialog
  open={deleteConversationsOpen}
  title="Gesprekken verwijderen"
  message="Weet je zeker dat je alle gesprekken wilt verwijderen? Dit kan niet ongedaan worden gemaakt."
  confirmLabel="Verwijderen"
  danger={true}
  onconfirm={handleDeleteConversations}
  oncancel={() => (deleteConversationsOpen = false)}
/>

<ConfirmDialog
  open={deleteEmbeddingsOpen}
  title="Embeddings verwijderen"
  message="Weet je zeker dat je alle embeddings wilt verwijderen? Dit kan niet ongedaan worden gemaakt."
  confirmLabel="Verwijderen"
  danger={true}
  onconfirm={handleDeleteEmbeddings}
  oncancel={() => (deleteEmbeddingsOpen = false)}
/>
