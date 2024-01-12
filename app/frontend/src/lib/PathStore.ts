import { writable, derived } from 'svelte/store'

const angle = writable(0)
const angleInRadians: any = derived($angle, (($angle + 270) * Math.PI) / 180)
const positionX = writable(0)
const positionY = writable(0)

export { angle, angleInRadians, positionX, positionY }