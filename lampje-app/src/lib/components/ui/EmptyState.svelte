<script lang="ts">
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  let {
    icon: Icon,
    title,
    description,
    actionLabel,
    onaction
  }: {
    icon?: any;
    title: string;
    description?: string;
    actionLabel?: string;
    onaction?: () => void;
  } = $props();
</script>

<div class="flex flex-col items-center py-12 text-center">
  {#if Icon}
    <div class="text-on-bg-muted/40">
      <Icon size={48} strokeWidth={1.5} />
    </div>
  {/if}

  <h3 class="font-display text-lg text-on-bg-dim {Icon ? 'mt-4' : ''}">
    {title}
  </h3>

  {#if description}
    <p class="mt-2 max-w-xs font-body text-sm text-on-bg-muted">
      {description}
    </p>
  {/if}

  {#if actionLabel && onaction}
    <button
      type="button"
      class="mt-4 cursor-pointer rounded-xl bg-primary px-4 py-2 font-body text-sm text-on-primary transition-colors duration-150 hover:bg-primary/80"
      onclick={onaction}
    >
      {actionLabel}
    </button>
  {/if}
</div>
