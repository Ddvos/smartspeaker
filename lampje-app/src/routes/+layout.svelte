<script lang="ts">
  import './layout.css';
  import favicon from '$lib/assets/favicon.svg';
  import Sidebar from '$lib/components/layout/Sidebar.svelte';
  import BottomNav from '$lib/components/layout/BottomNav.svelte';
  import PageTransition from '$lib/components/layout/PageTransition.svelte';
  import { page } from '$app/state';

  let { children } = $props();
</script>

<svelte:head>
  <link rel="icon" href={favicon} />
</svelte:head>

<div class="flex h-screen overflow-hidden bg-background">
  <!-- Desktop sidebar -->
  <div class="hidden md:block">
    <Sidebar currentPath={page.url.pathname} />
  </div>

  <!-- Main content area -->
  <main class="flex-1 overflow-y-auto p-6 lg:p-8">
    <PageTransition key={page.url.pathname}>
      {@render children()}
    </PageTransition>
  </main>
</div>

<!-- Mobile bottom nav -->
<div class="block md:hidden fixed bottom-0 left-0 right-0 z-40">
  <BottomNav currentPath={page.url.pathname} />
</div>
