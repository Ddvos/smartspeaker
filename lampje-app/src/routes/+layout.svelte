<script lang="ts">
  import './layout.css';
  import favicon from '$lib/assets/favicon.svg';
  import { page } from '$app/state';
  import { browser } from '$app/environment';
  import posthog from 'posthog-js';
  import CookieBanner from '$lib/components/ui/CookieBanner.svelte';

  let { children } = $props();

  // Capture SPA pageviews on every client-side navigation
  $effect(() => {
    if (browser && page.url) {
      posthog.capture('$pageview');
    }
  });
</script>

<svelte:head>
  <link rel="icon" href={favicon} />
</svelte:head>

{@render children()}
<CookieBanner />
