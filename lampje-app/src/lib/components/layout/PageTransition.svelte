<script lang="ts">
  import type { Snippet } from 'svelte';

  let { key, children }: { key: string; children: Snippet } = $props();
</script>

{#key key}
  <div class="animate-fade-in-up" style="animation: fade-in-up 200ms ease-out both;">
    {@render children()}
  </div>
{/key}
