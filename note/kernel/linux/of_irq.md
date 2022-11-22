## irq_of_parse_and_map 与 of_irq_get
查看两个接口的源代码可以发现of_irq_get只是多了一个domain检测：
```
int of_irq_get(struct device_node *dev,  int index)
{
	rc = of_irq_parse_one(dev,  index,  &oirq);

	domain = irq_find_host(oirq.np);
	if (!domain)
		return -EPROBE_DEFER;

	return irq_create_of_mapping(&oirq);
}
```
