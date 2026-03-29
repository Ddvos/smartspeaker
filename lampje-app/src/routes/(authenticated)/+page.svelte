<script lang="ts">
  import { page } from '$app/state';
  import { mockStats } from '$lib/data/mock-dashboard';
  import GreetingHeader from '$lib/components/dashboard/GreetingHeader.svelte';
  import ActivityChart from '$lib/components/dashboard/ActivityChart.svelte';
  import RecentConversations from '$lib/components/dashboard/RecentConversations.svelte';
  import TopTopics from '$lib/components/dashboard/TopTopics.svelte';
  import VoiceStatus from '$lib/components/dashboard/VoiceStatus.svelte';
  import DeviceStatus from '$lib/components/dashboard/DeviceStatus.svelte';
  import ConfigPreview from '$lib/components/dashboard/ConfigPreview.svelte';
  import ApiStatus from '$lib/components/dashboard/ApiStatus.svelte';
  import SearchBar from '$lib/components/ui/SearchBar.svelte';
  import StatCard from '$lib/components/ui/StatCard.svelte';
  import type { Device } from '$lib/types';

  const data = $derived(page.data as { primaryDevice: Device | null; session: any });
</script>

<div class="space-y-6 lg:space-y-0 lg:flex lg:gap-8">
  <!-- Main column -->
  <div class="flex-1 min-w-0 space-y-6">
    <GreetingHeader userName={page.data.session?.user?.name ?? 'daar'} />
    <SearchBar placeholder="Doorzoek je gesprekken, kennis, en herinneringen..." showShortcut={true} />
    <div class="grid grid-cols-2 lg:grid-cols-4 gap-4">
      {#each mockStats as stat, i}
        <StatCard label={stat.label} value={stat.value} trend={stat.trend} icon={stat.icon} index={i} />
      {/each}
    </div>
    <ActivityChart />
    <div class="grid grid-cols-1 xl:grid-cols-2 gap-6">
      <RecentConversations />
      <TopTopics />
    </div>
  </div>

  <!-- Right sidebar -->
  <aside class="lg:w-[340px] lg:shrink-0 space-y-4">
    <VoiceStatus />
    <DeviceStatus device={data.primaryDevice} />
    <ConfigPreview />
    <ApiStatus />
  </aside>
</div>
