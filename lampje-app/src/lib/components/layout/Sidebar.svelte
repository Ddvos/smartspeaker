<script lang="ts">
  import {
    LayoutDashboard,
    MessageSquare,
    BookOpen,
    Mic,
    Cpu,
    Settings,
    HelpCircle
  } from 'lucide-svelte';
  import SidebarItem from './SidebarItem.svelte';

  let { currentPath }: { currentPath: string } = $props();
</script>

<nav
  class="flex h-full w-[68px] flex-col items-center gap-1 bg-surface-0 py-4"
  aria-label="Hoofdnavigatie"
>
  <!-- Brand mark -->
  <div
    class="mb-4 flex h-10 w-10 items-center justify-center rounded-xl bg-primary font-display text-lg font-bold text-on-primary"
  >
    L
  </div>

  <SidebarItem href="/" icon={LayoutDashboard} label="Dashboard" isActive={currentPath === '/'} />
  <SidebarItem
    href="/conversations"
    icon={MessageSquare}
    label="Gesprekken"
    isActive={currentPath.startsWith('/conversations')}
  />
  <SidebarItem
    href="/knowledge"
    icon={BookOpen}
    label="Kennisbank"
    isActive={currentPath.startsWith('/knowledge')}
  />
  <SidebarItem
    href="/voicelab"
    icon={Mic}
    label="Voice Lab"
    isActive={currentPath.startsWith('/voicelab')}
  />
  <SidebarItem
    href="/devices"
    icon={Cpu}
    label="Apparaten"
    isActive={currentPath.startsWith('/devices')}
  />
  <SidebarItem
    href="/settings"
    icon={Settings}
    label="Instellingen"
    isActive={currentPath.startsWith('/settings')}
  />

  <!-- Spacer -->
  <div class="flex-grow"></div>

  <SidebarItem
    href="/help"
    icon={HelpCircle}
    label="Help"
    isActive={currentPath.startsWith('/help')}
  />
</nav>
