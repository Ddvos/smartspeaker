<script lang="ts">
  // eslint-disable-next-line @typescript-eslint/no-explicit-any
  let {
    href,
    icon: Icon,
    label,
    isActive = false
  }: {
    href: string;
    icon: any;
    label: string;
    isActive?: boolean;
  } = $props();
</script>

<a
  {href}
  class="group relative flex items-center justify-center w-12 h-12 rounded-xl transition-all duration-150
    {isActive
    ? 'bg-primary/8 text-primary'
    : 'text-on-bg-muted hover:text-on-bg-dim hover:bg-surface-2/50'}"
  aria-label={label}
  aria-current={isActive ? 'page' : undefined}
>
  <Icon size={22} strokeWidth={1.5} />

  <!-- Tooltip -->
  <span
    class="pointer-events-none absolute left-full ml-3 whitespace-nowrap rounded-lg bg-surface-2 px-3 py-1.5 font-label text-xs text-on-bg opacity-0 transition-opacity duration-150 group-hover:opacity-100 z-50"
  >
    {label}
  </span>
</a>
