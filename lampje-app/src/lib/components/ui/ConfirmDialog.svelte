<script lang="ts">
  let {
    open = false,
    title,
    message,
    confirmLabel = 'Bevestigen',
    danger = false,
    onconfirm,
    oncancel
  }: {
    open?: boolean;
    title: string;
    message: string;
    confirmLabel?: string;
    danger?: boolean;
    onconfirm: () => void;
    oncancel: () => void;
  } = $props();

  let cancelBtn: HTMLButtonElement | undefined = $state();

  $effect(() => {
    if (open && cancelBtn) {
      cancelBtn.focus();
    }
  });

  function handleOverlayClick(e: MouseEvent) {
    if (e.target === e.currentTarget) {
      oncancel();
    }
  }

  function handleKeydown(e: KeyboardEvent) {
    if (e.key === 'Escape') {
      oncancel();
    }
  }
</script>

<style>
  .dialog-enter {
    animation: dialog-in 200ms ease-out both;
  }

  @keyframes dialog-in {
    from {
      opacity: 0;
      transform: scale(0.95);
    }
    to {
      opacity: 1;
      transform: scale(1);
    }
  }
</style>

{#if open}
  <!-- svelte-ignore a11y_no_noninteractive_element_interactions -->
  <div
    class="fixed inset-0 z-50 flex items-center justify-center bg-black/60 backdrop-blur-sm"
    role="dialog"
    aria-modal="true"
    aria-labelledby="confirm-dialog-title"
    tabindex="-1"
    onclick={handleOverlayClick}
    onkeydown={handleKeydown}
  >
    <div class="dialog-enter mx-4 w-full max-w-md rounded-xl bg-surface-1/80 p-6 backdrop-blur-xl">
      <h2 id="confirm-dialog-title" class="font-display text-lg text-on-bg">
        {title}
      </h2>

      <p class="mt-2 font-body text-sm text-on-bg-dim">
        {message}
      </p>

      <div class="mt-6 flex justify-end gap-3">
        <button
          bind:this={cancelBtn}
          type="button"
          class="cursor-pointer rounded-xl bg-surface-2 px-4 py-2 font-body text-sm text-on-bg-dim transition-colors duration-150 hover:bg-surface-bright"
          onclick={oncancel}
        >
          Annuleren
        </button>

        <button
          type="button"
          class="cursor-pointer rounded-xl px-4 py-2 font-body text-sm font-medium transition-colors duration-150
            {danger
            ? 'bg-error text-on-bg hover:bg-error/80'
            : 'bg-primary text-on-primary hover:bg-primary/80'}"
          onclick={onconfirm}
        >
          {confirmLabel}
        </button>
      </div>
    </div>
  </div>
{/if}
