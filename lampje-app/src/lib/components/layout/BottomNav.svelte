<script lang="ts">
  import { LayoutDashboard, MessageSquare, Mic, BookOpen, Settings } from 'lucide-svelte';

  let { currentPath }: { currentPath: string } = $props();

  const items = [
    { href: '/', icon: LayoutDashboard, label: 'Dashboard', match: (p: string) => p === '/' },
    {
      href: '/conversations',
      icon: MessageSquare,
      label: 'Gesprekken',
      match: (p: string) => p.startsWith('/conversations')
    },
    {
      href: '/voicelab',
      icon: Mic,
      label: 'Voice Lab',
      match: (p: string) => p.startsWith('/voicelab')
    },
    {
      href: '/knowledge',
      icon: BookOpen,
      label: 'Kennisbank',
      match: (p: string) => p.startsWith('/knowledge')
    },
    {
      href: '/settings',
      icon: Settings,
      label: 'Instellingen',
      match: (p: string) => p.startsWith('/settings')
    }
  ];
</script>

<nav
  class="flex w-full items-center justify-around bg-surface-0/80 px-2 pb-[env(safe-area-inset-bottom)] pt-2 backdrop-blur-xl"
  aria-label="Mobiele navigatie"
>
  {#each items as item}
    {@const active = item.match(currentPath)}
    <a
      href={item.href}
      class="flex flex-col items-center gap-0.5 px-2 py-1.5 transition-colors duration-150
        {active ? 'text-primary' : 'text-on-bg-muted'}"
      aria-label={item.label}
      aria-current={active ? 'page' : undefined}
    >
      <item.icon size={20} strokeWidth={1.5} />
      <span class="font-label text-[10px] leading-tight">{item.label}</span>
    </a>
  {/each}
</nav>
