; ModuleID = 'orderedTwo.bc'
source_filename = "unorderedTwo.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.nodeOneOld = type { double, [6 x i8], i8, i32, i32 }

@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"a: %f\0A---\0A\00", align 1
@.str.2 = private unnamed_addr constant [13 x i8] c"b: %lld\0A---\0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"c: %d\0A---\0A\00", align 1
@.str.4 = private unnamed_addr constant [11 x i8] c"d: %d\0A---\0A\00", align 1
@.str.5 = private unnamed_addr constant [11 x i8] c"e: %c\0A---\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateNodeOneOld(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %2, i64 %idxprom
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  store double 1.700000e+01, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneOld, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 1
  %bf.load = load i64, ptr %b, align 8
  %bf.clear = and i64 %bf.load, -281474976710656
  %bf.set = or i64 %bf.clear, 1000
  %6 = trunc i64 %bf.set to i48
  store i48 %6, ptr %b, align 8
  %7 = load ptr, ptr %array.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %8 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneOld, ptr %7, i64 %idxprom3
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 3
  store i32 77, ptr %c, align 8
  %9 = load ptr, ptr %array.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %10 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %9, i64 %idxprom5
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 4
  store i32 10, ptr %d, align 4
  %11 = load ptr, ptr %array.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %12 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %idxprom7
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 2
  store i8 100, ptr %e, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %13 = load i32, ptr %i, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneOld(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %array.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %2, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 1
  %bf.load = load i64, ptr %b, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %conv = sitofp i64 %bf.ashr to double
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneOld, ptr %4, i64 %idxprom1
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 0
  %6 = load double, ptr %a, align 8
  %mul = fmul double %conv, %6
  %div = fdiv double %mul, 2.000000e+01
  %7 = load ptr, ptr %array.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %8 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneOld, ptr %7, i64 %idxprom3
  %a5 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 0
  store double %div, ptr %a5, align 8
  %9 = load ptr, ptr %array.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom6 = sext i32 %10 to i64
  %arrayidx7 = getelementptr inbounds %struct.nodeOneOld, ptr %9, i64 %idxprom6
  %a8 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx7, i32 0, i32 0
  %11 = load double, ptr %a8, align 8
  %12 = load ptr, ptr %array.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom9 = sext i32 %13 to i64
  %arrayidx10 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom9
  %b11 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx10, i32 0, i32 1
  %bf.load12 = load i64, ptr %b11, align 8
  %bf.shl13 = shl i64 %bf.load12, 16
  %bf.ashr14 = ashr i64 %bf.shl13, 16
  %conv15 = sitofp i64 %bf.ashr14 to double
  %sub = fsub double %11, %conv15
  %mul16 = fmul double %sub, 2.000000e+01
  %conv17 = fptosi double %mul16 to i64
  %14 = load ptr, ptr %array.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom18 = sext i32 %15 to i64
  %arrayidx19 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i64 %idxprom18
  %b20 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx19, i32 0, i32 1
  %bf.load21 = load i64, ptr %b20, align 8
  %bf.value = and i64 %conv17, 281474976710655
  %bf.clear = and i64 %bf.load21, -281474976710656
  %bf.set = or i64 %bf.clear, %bf.value
  %16 = trunc i64 %bf.set to i48
  store i48 %16, ptr %b20, align 8
  %bf.result.shl = shl i64 %bf.value, 16
  %bf.result.ashr = ashr i64 %bf.result.shl, 16
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %17 = load i32, ptr %i, align 4
  %inc = add nsw i32 %17, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !8

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multArrays(ptr noundef %arrayOne, ptr noundef %arrayTwo, i32 noundef %size) #0 {
entry:
  %arrayOne.addr = alloca ptr, align 8
  %arrayTwo.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %arrayOne, ptr %arrayOne.addr, align 8
  store ptr %arrayTwo, ptr %arrayTwo.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 1000
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %size.addr, align 4
  %cmp2 = icmp slt i32 %1, %2
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %3 = load ptr, ptr %arrayOne.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %3, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 1
  %bf.load = load i64, ptr %b, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %5 = load ptr, ptr %arrayTwo.addr, align 8
  %6 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %6 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %5, i64 %idxprom4
  %b6 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 1
  %bf.load7 = load i64, ptr %b6, align 8
  %bf.shl8 = shl i64 %bf.load7, 16
  %bf.ashr9 = ashr i64 %bf.shl8, 16
  %mul = mul nsw i64 %bf.ashr, %bf.ashr9
  %7 = load ptr, ptr %arrayOne.addr, align 8
  %8 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %8 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneOld, ptr %7, i64 %idxprom10
  %b12 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 1
  %bf.load13 = load i64, ptr %b12, align 8
  %bf.value = and i64 %mul, 281474976710655
  %bf.clear = and i64 %bf.load13, -281474976710656
  %bf.set = or i64 %bf.clear, %bf.value
  %9 = trunc i64 %bf.set to i48
  store i48 %9, ptr %b12, align 8
  %bf.result.shl = shl i64 %bf.value, 16
  %bf.result.ashr = ashr i64 %bf.result.shl, 16
  %10 = load ptr, ptr %arrayOne.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom14 = sext i32 %11 to i64
  %arrayidx15 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom14
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx15, i32 0, i32 2
  store i8 102, ptr %e, align 8
  %12 = load ptr, ptr %arrayTwo.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %13 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom16
  %e18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 2
  store i8 102, ptr %e18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %14 = load i32, ptr %i, align 4
  %inc = add nsw i32 %14, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !9

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %15 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %15, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !10

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %arrayOneOld = alloca ptr, align 8
  %arrayTwoOld = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  store i32 9999999, ptr %n, align 4
  %0 = load i32, ptr %n, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 32
  %call = call noalias ptr @malloc(i64 noundef %mul) #4
  store ptr %call, ptr %arrayOneOld, align 8
  %1 = load i32, ptr %n, align 4
  %conv1 = sext i32 %1 to i64
  %mul2 = mul i64 %conv1, 32
  %call3 = call noalias ptr @malloc(i64 noundef %mul2) #4
  store ptr %call3, ptr %arrayTwoOld, align 8
  %2 = load ptr, ptr %arrayOneOld, align 8
  %3 = load i32, ptr %n, align 4
  call void @populateNodeOneOld(ptr noundef %2, i32 noundef %3)
  %4 = load ptr, ptr %arrayTwoOld, align 8
  %5 = load i32, ptr %n, align 4
  call void @populateNodeOneOld(ptr noundef %4, i32 noundef %5)
  %6 = load ptr, ptr %arrayOneOld, align 8
  %7 = load i32, ptr %n, align 4
  call void @multNodeOneOld(ptr noundef %6, i32 noundef %7)
  %8 = load ptr, ptr %arrayOneOld, align 8
  %9 = load ptr, ptr %arrayTwoOld, align 8
  %10 = load i32, ptr %n, align 4
  call void @multArrays(ptr noundef %8, ptr noundef %9, i32 noundef %10)
  %call4 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %11 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 10
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %12 = load double, ptr %a, align 8
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, double noundef %12)
  %13 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %13, i64 10
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 1
  %bf.load = load i64, ptr %b, align 8
  %bf.shl = shl i64 %bf.load, 16
  %bf.ashr = ashr i64 %bf.shl, 16
  %call7 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i64 noundef %bf.ashr)
  %14 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i64 10
  %c = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 3
  %15 = load i32, ptr %c, align 8
  %call9 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %15)
  %16 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx10 = getelementptr inbounds %struct.nodeOneOld, ptr %16, i64 10
  %d = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx10, i32 0, i32 4
  %17 = load i32, ptr %d, align 4
  %call11 = call i32 (ptr, ...) @printf(ptr noundef @.str.4, i32 noundef %17)
  %18 = load ptr, ptr %arrayOneOld, align 8
  %arrayidx12 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 10
  %e = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx12, i32 0, i32 2
  %19 = load i8, ptr %e, align 8
  %conv13 = sext i8 %19 to i32
  %call14 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %conv13)
  %20 = load ptr, ptr %arrayOneOld, align 8
  call void @free(ptr noundef %20) #5
  %21 = load ptr, ptr %arrayTwoOld, align 8
  call void @free(ptr noundef %21) #5
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare noalias ptr @malloc(i64 noundef) #1

declare i32 @printf(ptr noundef, ...) #2

; Function Attrs: nounwind
declare void @free(ptr noundef) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0) }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (https://github.com/llvm/llvm-project.git 893416051d517d979481510fe3b52d83ba0d1e01)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
