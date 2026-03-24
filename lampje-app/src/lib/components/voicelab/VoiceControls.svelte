<script lang="ts">
  import { Square, Plus } from 'lucide-svelte';
  import { voiceStore } from '$lib/stores/voice.svelte';
</script>

{#if voiceStore.state !== 'idle'}
  <div class="mt-8 flex items-center gap-4">
    <button
      type="button"
      class="flex h-12 w-12 cursor-pointer items-center justify-center rounded-full bg-error/20 text-error transition-colors duration-150 hover:bg-error/30"
      onclick={() => voiceStore.stop()}
      aria-label="Stop"
    >
      <Square size={18} />
    </button>

    <button
      type="button"
      class="flex cursor-pointer items-center gap-2 rounded-xl bg-surface-2 px-4 py-2.5 font-body text-sm text-on-bg-dim transition-colors duration-150 hover:bg-surface-bright"
      onclick={() => voiceStore.reset()}
    >
      <Plus size={16} />
      Nieuw gesprek
    </button>
  </div>
{/if}
