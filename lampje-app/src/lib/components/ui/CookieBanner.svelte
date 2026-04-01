<script lang="ts">
  import { Cookie } from 'lucide-svelte';
  import { consentStore } from '$lib/stores/consent.svelte';
</script>

<style>
  .banner-enter {
    animation: fade-in-up 300ms ease-out both;
  }
</style>

{#if consentStore.status === 'pending'}
  <div class="fixed inset-x-0 bottom-0 z-50 flex justify-center p-4">
    <div
      class="banner-enter flex w-full max-w-lg flex-col gap-4 rounded-xl bg-surface-1/90 p-5 shadow-glow-sm backdrop-blur-xl"
    >
      <div class="flex items-start gap-3">
        <Cookie size={20} class="mt-0.5 shrink-0 text-primary" />
        <div>
          <h3 class="font-display text-sm font-medium text-on-bg">Cookies voor analytics</h3>
          <p class="mt-1 font-body text-xs leading-relaxed text-on-bg-dim">
            We gebruiken analytics cookies om te begrijpen hoe de app wordt gebruikt en deze te
            verbeteren. Zonder cookies verzamelen we alleen anonieme, geaggregeerde data.
          </p>
        </div>
      </div>

      <div class="flex gap-3">
        <button
          type="button"
          class="flex-1 cursor-pointer rounded-xl bg-surface-2 px-4 py-2.5 font-body text-sm font-medium text-on-bg transition-colors duration-150 hover:bg-surface-bright"
          onclick={() => consentStore.decline()}
        >
          Weigeren
        </button>
        <button
          type="button"
          class="flex-1 cursor-pointer rounded-xl bg-primary px-4 py-2.5 font-body text-sm font-medium text-on-primary transition-colors duration-150 hover:bg-primary-dim"
          onclick={() => consentStore.accept()}
        >
          Accepteren
        </button>
      </div>
    </div>
  </div>
{/if}
