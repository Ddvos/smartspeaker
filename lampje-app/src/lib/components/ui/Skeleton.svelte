<script lang="ts">
  let {
    width = '100%',
    height = '1rem',
    rounded = 'lg'
  }: {
    width?: string;
    height?: string;
    rounded?: string;
  } = $props();

  const borderRadius = $derived(
    rounded === 'full'
      ? '9999px'
      : rounded === 'xl'
        ? '1.5rem'
        : rounded === 'lg'
          ? '1rem'
          : rounded === 'md'
            ? '0.5rem'
            : rounded === 'sm'
              ? '0.25rem'
              : rounded
  );
</script>

<div
  class="skeleton"
  style="width: {width}; height: {height}; border-radius: {borderRadius};"
  aria-hidden="true"
></div>

<style>
  .skeleton {
    background: linear-gradient(
      90deg,
      var(--color-surface-2) 25%,
      var(--color-surface-bright) 50%,
      var(--color-surface-2) 75%
    );
    background-size: 200% 100%;
    animation: shimmer 1.5s infinite;
  }

  @keyframes shimmer {
    0% {
      background-position: -200% 0;
    }
    100% {
      background-position: 200% 0;
    }
  }
</style>
