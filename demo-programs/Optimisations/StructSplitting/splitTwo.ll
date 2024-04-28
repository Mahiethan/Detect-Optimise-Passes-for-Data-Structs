; ModuleID = 'splitTwo.c'
source_filename = "splitTwo.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.nodeOneOld = type { i32, double, ptr, ptr }
%struct.nodeOneCold = type { i32, double, i8, double, double, double, double, float }

@.str = private unnamed_addr constant [16 x i8] c"Validity check\0A\00", align 1
@.str.1 = private unnamed_addr constant [8 x i8] c"%d\0A---\0A\00", align 1
@.str.2 = private unnamed_addr constant [8 x i8] c"%f\0A---\0A\00", align 1
@.str.3 = private unnamed_addr constant [8 x i8] c"%c\0A---\0A\00", align 1

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
  store i32 1, ptr %a, align 8
  %4 = load ptr, ptr %array.addr, align 8
  %5 = load i32, ptr %i, align 4
  %idxprom1 = sext i32 %5 to i64
  %arrayidx2 = getelementptr inbounds %struct.nodeOneOld, ptr %4, i64 %idxprom1
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx2, i32 0, i32 1
  store double 1.000000e+01, ptr %b, align 8
  %call = call noalias ptr @malloc(i64 noundef 64) #5
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom3 = sext i32 %7 to i64
  %arrayidx4 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom3
  %cold = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx4, i32 0, i32 2
  store ptr %call, ptr %cold, align 8
  %8 = load ptr, ptr %array.addr, align 8
  %9 = load i32, ptr %i, align 4
  %idxprom5 = sext i32 %9 to i64
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %8, i64 %idxprom5
  %cold7 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 2
  %10 = load ptr, ptr %cold7, align 8
  %c = getelementptr inbounds %struct.nodeOneCold, ptr %10, i32 0, i32 0
  store i32 9, ptr %c, align 8
  %11 = load ptr, ptr %array.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom8 = sext i32 %12 to i64
  %arrayidx9 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %idxprom8
  %cold10 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx9, i32 0, i32 2
  %13 = load ptr, ptr %cold10, align 8
  %d = getelementptr inbounds %struct.nodeOneCold, ptr %13, i32 0, i32 1
  store double 2.300000e+01, ptr %d, align 8
  %14 = load ptr, ptr %array.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom11 = sext i32 %15 to i64
  %arrayidx12 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i64 %idxprom11
  %cold13 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx12, i32 0, i32 2
  %16 = load ptr, ptr %cold13, align 8
  %e = getelementptr inbounds %struct.nodeOneCold, ptr %16, i32 0, i32 2
  store i8 97, ptr %e, align 8
  %17 = load ptr, ptr %array.addr, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom14 = sext i32 %18 to i64
  %arrayidx15 = getelementptr inbounds %struct.nodeOneOld, ptr %17, i64 %idxprom14
  %cold16 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx15, i32 0, i32 2
  %19 = load ptr, ptr %cold16, align 8
  %f = getelementptr inbounds %struct.nodeOneCold, ptr %19, i32 0, i32 3
  store double 2.300000e+01, ptr %f, align 8
  %20 = load ptr, ptr %array.addr, align 8
  %21 = load i32, ptr %i, align 4
  %idxprom17 = sext i32 %21 to i64
  %arrayidx18 = getelementptr inbounds %struct.nodeOneOld, ptr %20, i64 %idxprom17
  %cold19 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx18, i32 0, i32 2
  %22 = load ptr, ptr %cold19, align 8
  %g = getelementptr inbounds %struct.nodeOneCold, ptr %22, i32 0, i32 4
  store double 2.300000e+01, ptr %g, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %23 = load i32, ptr %i, align 4
  %inc = add nsw i32 %23, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @multNodeOneOld(ptr noundef %array, i32 noundef %size) #0 {
entry:
  %array.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %array, ptr %array.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, ptr %j, align 4
  %cmp = icmp slt i32 %0, 40
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
  %3 = load ptr, ptr %array.addr, align 8
  %4 = load i32, ptr %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %3, i64 %idxprom
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 1
  %5 = load double, ptr %b, align 8
  %6 = load ptr, ptr %array.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom4
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 0
  %8 = load i32, ptr %a, align 8
  %conv = sitofp i32 %8 to double
  %9 = call double @llvm.fmuladd.f64(double %5, double %conv, double 2.000000e+01)
  %conv6 = fptosi double %9 to i32
  %10 = load ptr, ptr %array.addr, align 8
  %11 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %10, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %conv6, ptr %a9, align 8
  %12 = load ptr, ptr %array.addr, align 8
  %13 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %13 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneOld, ptr %12, i64 %idxprom10
  %a12 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 0
  %14 = load i32, ptr %a12, align 8
  %15 = load ptr, ptr %array.addr, align 8
  %16 = load i32, ptr %i, align 4
  %idxprom13 = sext i32 %16 to i64
  %arrayidx14 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 %idxprom13
  %cold = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx14, i32 0, i32 2
  %17 = load ptr, ptr %cold, align 8
  %c = getelementptr inbounds %struct.nodeOneCold, ptr %17, i32 0, i32 0
  %18 = load i32, ptr %c, align 8
  %div = sdiv i32 %14, %18
  %sub = sub nsw i32 %div, 297
  %conv15 = sitofp i32 %sub to double
  %19 = load ptr, ptr %array.addr, align 8
  %20 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %20 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %19, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double %conv15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %21 = load i32, ptr %i, align 4
  %inc = add nsw i32 %21, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !6

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %22 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %22, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !7

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare double @llvm.fmuladd.f64(double, double, double) #2

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
  %cmp = icmp slt i32 %0, 30
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
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %5 = load i32, ptr %a, align 8
  %6 = load ptr, ptr %arrayTwo.addr, align 8
  %7 = load i32, ptr %i, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds %struct.nodeOneOld, ptr %6, i64 %idxprom4
  %a6 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx5, i32 0, i32 0
  %8 = load i32, ptr %a6, align 8
  %mul = mul nsw i32 %5, %8
  %9 = load ptr, ptr %arrayOne.addr, align 8
  %10 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %10 to i64
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %9, i64 %idxprom7
  %a9 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 0
  store i32 %mul, ptr %a9, align 8
  %11 = load ptr, ptr %arrayOne.addr, align 8
  %12 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %12 to i64
  %arrayidx11 = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 %idxprom10
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx11, i32 0, i32 1
  %13 = load double, ptr %b, align 8
  %14 = load ptr, ptr %arrayTwo.addr, align 8
  %15 = load i32, ptr %i, align 4
  %idxprom12 = sext i32 %15 to i64
  %arrayidx13 = getelementptr inbounds %struct.nodeOneOld, ptr %14, i64 %idxprom12
  %b14 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx13, i32 0, i32 1
  %16 = load double, ptr %b14, align 8
  %mul15 = fmul double %13, %16
  %17 = load ptr, ptr %arrayOne.addr, align 8
  %18 = load i32, ptr %i, align 4
  %idxprom16 = sext i32 %18 to i64
  %arrayidx17 = getelementptr inbounds %struct.nodeOneOld, ptr %17, i64 %idxprom16
  %b18 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx17, i32 0, i32 1
  store double %mul15, ptr %b18, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %19 = load i32, ptr %i, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !8

for.end:                                          ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end
  %20 = load i32, ptr %j, align 4
  %inc20 = add nsw i32 %20, 1
  store i32 %inc20, ptr %j, align 4
  br label %for.cond, !llvm.loop !9

for.end21:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @freeAoS(ptr noundef %aos) #0 {
entry:
  %aos.addr = alloca ptr, align 8
  %arrayOneOldSize = alloca i32, align 4
  %currElem = alloca i32, align 4
  store ptr %aos, ptr %aos.addr, align 8
  %0 = load ptr, ptr %aos.addr, align 8
  %call = call i64 @malloc_usable_size(ptr noundef %0) #6
  %div = udiv i64 %call, 32
  %conv = trunc i64 %div to i32
  store i32 %conv, ptr %arrayOneOldSize, align 4
  store i32 0, ptr %currElem, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %1 = load i32, ptr %currElem, align 4
  %2 = load i32, ptr %arrayOneOldSize, align 4
  %cmp = icmp ne i32 %1, %2
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %3 = load ptr, ptr %aos.addr, align 8
  %4 = load i32, ptr %currElem, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %3, i64 %idxprom
  %cold = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 2
  %5 = load ptr, ptr %cold, align 8
  call void @free(ptr noundef %5) #6
  %6 = load i32, ptr %currElem, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, ptr %currElem, align 4
  br label %while.cond, !llvm.loop !10

while.end:                                        ; preds = %while.cond
  %7 = load ptr, ptr %aos.addr, align 8
  call void @free(ptr noundef %7) #6
  ret void
}

; Function Attrs: nounwind
declare dso_local i64 @malloc_usable_size(ptr noundef) #3

; Function Attrs: nounwind
declare dso_local void @free(ptr noundef) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %arrayOneHot = alloca ptr, align 8
  %arrayTwoHot = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  store i32 999999, ptr %n, align 4
  %0 = load i32, ptr %n, align 4
  %conv = sext i32 %0 to i64
  %mul = mul i64 %conv, 32
  %call = call noalias ptr @malloc(i64 noundef %mul) #5
  store ptr %call, ptr %arrayOneHot, align 8
  %1 = load i32, ptr %n, align 4
  %conv1 = sext i32 %1 to i64
  %mul2 = mul i64 %conv1, 32
  %call3 = call noalias ptr @malloc(i64 noundef %mul2) #5
  store ptr %call3, ptr %arrayTwoHot, align 8
  %2 = load ptr, ptr %arrayOneHot, align 8
  %3 = load i32, ptr %n, align 4
  call void @populateNodeOneOld(ptr noundef %2, i32 noundef %3)
  %4 = load ptr, ptr %arrayTwoHot, align 8
  %5 = load i32, ptr %n, align 4
  call void @populateNodeOneOld(ptr noundef %4, i32 noundef %5)
  %6 = load ptr, ptr %arrayOneHot, align 8
  %7 = load i32, ptr %n, align 4
  call void @multNodeOneOld(ptr noundef %6, i32 noundef %7)
  %8 = load ptr, ptr %arrayOneHot, align 8
  %9 = load ptr, ptr %arrayTwoHot, align 8
  %10 = load i32, ptr %n, align 4
  call void @multArrays(ptr noundef %8, ptr noundef %9, i32 noundef %10)
  %call4 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %11 = load ptr, ptr %arrayOneHot, align 8
  %arrayidx = getelementptr inbounds %struct.nodeOneOld, ptr %11, i64 5
  %a = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx, i32 0, i32 0
  %12 = load i32, ptr %a, align 8
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %12)
  %13 = load ptr, ptr %arrayOneHot, align 8
  %arrayidx6 = getelementptr inbounds %struct.nodeOneOld, ptr %13, i64 5
  %b = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx6, i32 0, i32 1
  %14 = load double, ptr %b, align 8
  %call7 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %14)
  %15 = load ptr, ptr %arrayOneHot, align 8
  %arrayidx8 = getelementptr inbounds %struct.nodeOneOld, ptr %15, i64 5
  %cold = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx8, i32 0, i32 2
  %16 = load ptr, ptr %cold, align 8
  %d = getelementptr inbounds %struct.nodeOneCold, ptr %16, i32 0, i32 1
  %17 = load double, ptr %d, align 8
  %call9 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, double noundef %17)
  %18 = load ptr, ptr %arrayOneHot, align 8
  %arrayidx10 = getelementptr inbounds %struct.nodeOneOld, ptr %18, i64 5
  %cold11 = getelementptr inbounds %struct.nodeOneOld, ptr %arrayidx10, i32 0, i32 2
  %19 = load ptr, ptr %cold11, align 8
  %e = getelementptr inbounds %struct.nodeOneCold, ptr %19, i32 0, i32 2
  %20 = load i8, ptr %e, align 8
  %conv12 = sext i8 %20 to i32
  %call13 = call i32 (ptr, ...) @printf(ptr noundef @.str.3, i32 noundef %conv12)
  %21 = load ptr, ptr %arrayOneHot, align 8
  call void @freeAoS(ptr noundef %21)
  %22 = load ptr, ptr %arrayTwoHot, align 8
  call void @freeAoS(ptr noundef %22)
  ret i32 0
}

declare dso_local i32 @printf(ptr noundef, ...) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind allocsize(0) }
attributes #6 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 16.0.6 (Red Hat 16.0.6-2.module+el8.9.0+1651+e10a8f6d)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
!9 = distinct !{!9, !5}
!10 = distinct !{!10, !5}
