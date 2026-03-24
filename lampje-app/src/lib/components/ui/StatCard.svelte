<script lang="ts">
  import { MessageSquare, Clock, Hash, Brain } from 'lucide-svelte';

  let {
    label,
    value,
    trend,
    icon,
    index = 0
  }: {
    label: string;
    value: string;
    trend?: string;
    icon?: string;
    index?: number;
  } = $props();

  const iconMap: Record<string, typeof MessageSquare> = {
    'message-square': MessageSquare,
    clock: Clock,
    hash: Hash,
    brain: Brain
  };

  const IconComponent = $derived(icon ? iconMap[icon] : undefined);
</script>

<div
  class="relative rounded-xl bg-surface-2 p-5 transition-colors duration-150 hover:bg-surface-bright"
  style="animation: stagger-in 400ms ease-out both; animation-delay: {index * 60}ms;"
>
  {#if IconComponent}
    {@const Icon = IconComponent}
    <div class="absolute right-5 top-5 text-on-bg-muted/30">
      <Icon size={28} strokeWidth={1.5} />
    </div>
  {/if}

  <p class="font-label text-xs uppercase tracking-wider text-on-bg-muted">
    {label}
  </p>

  <p class="mt-1 font-display text-2xl font-semibold text-on-bg">
    {value}
  </p>

  {#if trend}
    <p class="mt-2 font-label text-xs text-on-bg-dim">
      {trend}
    </p>
  {/if}
</div>
