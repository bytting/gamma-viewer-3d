if(energy <= 0.11)
{
	-16772 * Math.pow(energy, 5) + 7316.5 * Math.pow(energy, 4) - 1268.6 * Math.pow(energy, 3) + 109.49 * Math.pow(energy, 2) - 4.7059 * energy + 0.0822
}
else
{
	0.0013 * Math.pow(energy, 6) - 0.0142 * Math.pow(energy, 5) + 0.0629 * Math.pow(energy, 4) - 0.138 * Math.pow(energy, 3) + 0.1441 * Math.pow(energy, 2) - 0.0197 * energy + 0.0028
}